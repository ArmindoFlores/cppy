#ifndef _PYTRACEBACK_H_
#define _PYTRACEBACK_H_

#include <list>
#include <string>
#include <memory>

#define TB cpy::globals::Traceback::the()

namespace cpy {
    namespace globals {
        class traceback_exception : public std::exception {
        public:
            traceback_exception(const std::string& signature);
            const char* what() const noexcept override;

        private:
            std::string signature;
        };

        class Traceback {
        public:
            static Traceback &the();
            void push(const std::string&);
            void pop();
            void raise(const std::string& msg, const std::string& signature);

        private:
            static std::shared_ptr<Traceback> instance;
            Traceback();

        protected:
            std::list<std::string> tb;
            std::string create_message(const std::string& msg) const;
        };
    }
}

#endif