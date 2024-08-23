#ifndef FSM_hpp
#define FSM_hpp

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <iostream>
#include <thread>



namespace messaging
{
    struct message_base
    {
        virtual ~message_base(){};
    };

    template<typename Msg>
    struct wrapped_message: message_base
    {
        Msg contents;
        explicit wrapped_message(const Msg& cont):contents(cont){}
    };

    class queue
    {
        std::mutex m;
        std::condition_variable c;
        std::queue<std::shared_ptr<message_base>> q;
    public:
        
        template<typename T>
        void push(const T& message)
        {
            std::lock_guard lk(m);
            q.push(std::make_shared<wrapped_message<T>>(message));
            c.notify_all();
        }

        std::shared_ptr<message_base> wait_and_pop()
        {
            std::unique_lock ulk(m);
            c.wait(ulk, [&](){
                return !q.empty();
            });
            
            auto res = q.front();    
            q.pop();
            return res;
        } 
    };


    class sender
    {
        std::shared_ptr<queue> q;

    public:
        sender():q(nullptr){}
        
        explicit sender(const std::shared_ptr<queue> _q):q(_q){}
        
        template<typename Message>
        void send(const Message& msg)
        {
            if(q)
                q->push(msg);
        }
    };

   
    class close_queue{};
    
    template<
        typename PreviousDispatcher,
        typename Msg,
        typename Func>
    class TemplateDispatcher
    {
        std::shared_ptr<queue> q;
        PreviousDispatcher* prev;
        Func callable;
        bool chained;
        
        TemplateDispatcher(const TemplateDispatcher&)=delete;
        TemplateDispatcher& operator=(const TemplateDispatcher&)=delete;
        
        template<
            typename Dispatcher,
            typename OtherMsg,
            typename OtherFunc>
        friend class TemplateDispatcher;
        
        void wait_and_dispatch()
        {
            while(1)
            {
                auto msg = q->wait_and_pop();
                if(dispatch(msg))
                    break;
            }
        } 
    
        bool dispatch(const std::shared_ptr<message_base>& msg)
        {
            if(wrapped_message<Msg>* wrapper=
                dynamic_cast<wrapped_message<Msg>*>(msg.get()))
            {
                callable(wrapper->contents);
                return true;
            }
            else 
            {   
                return prev->dispatch(msg);
        
            }
        }
    public:
        TemplateDispatcher(TemplateDispatcher&& other):
            q(other.q), prev(other.prev), callable(std::move(other.callable))
            {
                other.chained = true;
            }

        TemplateDispatcher(
            std::shared_ptr<queue> _q, 
            PreviousDispatcher* _prev, 
            Func&& _callable): 
            q(_q), 
            prev(_prev), 
            callable(std::forward<Func>(_callable)), 
            chained(false) 
            { 
                _prev->chained = true; 
            }

        template<
            typename OtherMsg,
            typename OtherFunc>
        TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
        handle(OtherFunc&& of)
        {
            return TemplateDispatcher<
            TemplateDispatcher,
            OtherMsg, 
            OtherFunc>(q, this, std::forward<OtherFunc>(of));
        }

         ~TemplateDispatcher() noexcept(false)
        {
            if(!chained)
            {
                wait_and_dispatch();
            }
        }
    };
    
    class dispatcher
    {
        std::shared_ptr<queue> q;
        bool chained;
        
        dispatcher(const  dispatcher&)=delete;
        dispatcher& operator=(const dispatcher&)=delete;

        template<
        typename Dispatcher,
        typename Msg,
        typename Func>
        friend class TemplateDispatcher;

        void wait_and_dispatch()
        {
            while(1)
            {
                auto msg = q->wait_and_pop();
                dispatch(msg);
            }
        }

        bool dispatch(const std::shared_ptr<message_base>& msg)
        {
            if(dynamic_cast<wrapped_message<close_queue>*>(msg.get()))
            {
                throw close_queue();
            }    
            return false;
        }

    public:
        dispatcher(dispatcher&& other): q(other.q), chained(other.chained)
        {
            other.chained = true;
        }

        explicit dispatcher(const std::shared_ptr<queue>& _q):
            q(_q), 
            chained(false){}
        
        template<typename Message, typename Func> 
        TemplateDispatcher<dispatcher, Message, Func>
        handle(Func&& f)
        {
            return TemplateDispatcher<dispatcher, Message, Func>(
                q, 
                this, 
                std::forward<Func>(f)
            );
        }
        ~dispatcher() noexcept(false)
        {
            if(!chained)
            {   
                wait_and_dispatch();
            }
        }
    };
    
    class receiver
    {
        std::shared_ptr<queue> Q = std::make_shared<messaging::queue>();
    public:
        operator sender() { return sender(std::shared_ptr<queue>(Q)); }
        dispatcher wait() { return dispatcher(std::shared_ptr<queue>(Q)); } 
    };
}
 

#endif