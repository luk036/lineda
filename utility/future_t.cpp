#include <boost/shared_ptr.hpp>

#include <boost/bind.hpp>
#include <boost/type_traits.hpp>

#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>

namespace boost {
namespace detail {
template <typename R, typename F, typename L>
struct function_traits_helper<boost::_bi::bind_t<R, F, L> *> {
  BOOST_STATIC_CONSTANT(int, arity = 0);
  typedef R result_type;
};
}
}

class Worker; // forward declare

class FutureBase {
protected:
  FutureBase();

public:
  virtual ~FutureBase();

  void wait();
  boost::shared_ptr<std::string> exception();

private:
  volatile bool m_completed;
  boost::shared_ptr<std::string> m_exception;

  boost::mutex m_mutex;
  boost::condition m_control;

  friend class Worker;
};

template <typename R> class Future : public FutureBase {
private:
  struct Function {
    Function(boost::shared_ptr<Future<R> > j, boost::function0<R> f)
        : m_future(j), m_f(f) {}
    void operator()() { m_future->m_result = m_f(); }

  private:
    boost::shared_ptr<Future<R> > m_future;
    boost::function0<R> m_f;
  };

  Future() : m_result() {}

public:
  R result() {
    wait();
    return m_result;
  }

private:
  R m_result;
  friend class Worker;
};

class Worker : boost::noncopyable {
private:
  void execute();

public:
  // template< typename R > class Future;
  Worker();
  Worker(boost::function0<void> init);
  ~Worker();

  template <typename F>
  boost::shared_ptr<Future<typename boost::function_traits<
      typename boost::remove_pointer<F>::type>::result_type> >
  operator()(F f) {
    return run<typename boost::function_traits<
        typename boost::remove_pointer<F>::type>::result_type>(f);
  }

  template <typename R>
  boost::shared_ptr<Future<R> > run(boost::function<R(void)> f) {
    boost::shared_ptr<Future<R>> future(new Future<R>);
    queue(future, typename Future<R>::Function(future, f));
    return future;
  }

  void terminate();

private:
  void queue(boost::shared_ptr<FutureBase> j, boost::function0<void> f);

private:
  typedef std::list<
      std::pair<boost::shared_ptr<FutureBase>, boost::function0<void>>>
      t_queue;
  t_queue m_queue;
  volatile bool m_terminate;

  boost::mutex m_mutex;
  boost::condition m_control;
  boost::thread m_thread;
};

/*
  Worker
*/

Worker::Worker()
    : m_terminate(false), m_thread(boost::bind(&Worker::execute, this)) {}

Worker::~Worker() {
  {
    boost::mutex::scoped_lock lock(m_mutex);
    m_terminate = true;
    m_control.notify_all();
  }
  m_thread.join();
}

void Worker::queue(boost::shared_ptr<FutureBase> future,
                   boost::function0<void> f) {
  boost::mutex::scoped_lock lock(m_mutex);
  m_queue.push_back(std::make_pair(future, f));
  m_control.notify_all();
}

void Worker::execute() {
  do {
    t_queue job;
    { // Find a job to perform
      boost::mutex::scoped_lock lock(m_mutex);
      if (m_queue.empty())
        m_control.wait(lock);
      if (!m_terminate && !m_queue.empty())
        job.swap(m_queue);
    }
    for (t_queue::const_iterator j(job.begin()); j != job.end() && !m_terminate;
         ++j) {
      // Execute job
      try {
        j->second();
      } catch (std::exception &e) {
        boost::mutex::scoped_lock lock(j->first->m_mutex);
        j->first->m_exception =
            boost::shared_ptr<std::string>(new std::string(e.what()));
      } catch (...) {
        boost::mutex::scoped_lock lock(j->first->m_mutex);
        j->first->m_exception = boost::shared_ptr<std::string>(
            new std::string("An unknown exception was caught"));
      }
      // Notify futures
      boost::mutex::scoped_lock lock(j->first->m_mutex);
      j->first->m_completed = true;
      j->first->m_control.notify_all();
    }
  } while (!m_terminate);
}

/*
  Worker::Future< void >
*/

FutureBase::FutureBase() : m_completed(false) {}

FutureBase::~FutureBase() {}

boost::shared_ptr<std::string> FutureBase::exception() {
  boost::mutex::scoped_lock lock(m_mutex);
  m_control.wait(lock, boost::bind(&FutureBase::m_completed, this));
  return m_exception;
}

void FutureBase::wait() {
  boost::shared_ptr<std::string> e(exception());
  if (e)
    throw e;
}

/*
  Example
*/

#include <iostream>

int throws() { throw 0; }

int add(int l, int r) { return l + r; }

int main() {
  Worker worker1, worker2;

  boost::shared_ptr<Future<int>> job1(worker1(throws));
  int four(4), five(5);
  boost::shared_ptr<Future<int>> job2(worker2(boost::bind(add, four, five)));

  if (job1->exception())
    std::cout << "job1 threw " << *job1->exception() << std::endl;
  else
    std::cout << "job1 should have thrown an exception" << std::endl;

  std::cout << "4 + 5 = " << job2->result() << std::endl;
}
