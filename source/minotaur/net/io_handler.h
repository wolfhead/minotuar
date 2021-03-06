#ifndef _MINOTAUR_NET_IO_HANDLER_H_
#define _MINOTAUR_NET_IO_HANDLER_H_

#include "../event/timer/timer.h"
#include "../common/logger.h"
#include "../stage.h"
#include "../handler.h"
#include "io_message.h"

namespace ade {

class IOService;
class IOHandler;

class IOHandler : public Handler {
 public:
  typedef EventMessage* MessageType;
  typedef std::function<void()> TimerFunctor;
  typedef event::Timer<TimerFunctor> Timer;

  inline static uint32_t Hash(EventMessage* message) {
    return message->descriptor_id >> 48;
  }

  IOHandler(IOService* io_service);

  void Run(StageData<IOHandler>* data);

  void Handle(EventMessage* message);

  static IOHandler* GetCurrentIOHandler() {
    return current_io_handler_;
  }

  uint64_t StartTimer(uint32_t millisecond, const TimerFunctor& functor) {
    return timer_.AddTimer(millisecond, functor);
  }

  void CancelTimer(uint64_t timer_id) {
    timer_.CancelTimer(timer_id);
  }

  IOHandler* Clone();

 private:
  LOGGER_CLASS_DECL(logger);

  void ProcessTimer();

  void HandleIOReadEvent(EventMessage* message);

  void HandleIOWriteEvent(EventMessage* message);

  void HandleIOCloseEvent(EventMessage* message);

  void HandleIOMessageEvent(EventMessage* message);

  void HandleIOActiveCloseEvent(EventMessage* message);

  void HandleIOMessageFailure(EventMessage* message);

  Timer timer_;
  static thread_local IOHandler* current_io_handler_;
};


} // namespace ade

#endif // _MINOTAUR_NET_IO_HANDLER_H_
