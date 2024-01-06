#ifndef __UTILS_UTILS_H__
#define __UTILS_UTILS_H__

#include <chrono>
#include <future>
#include <string>
#include <string_view>
#include <regex>
#include <random>
#include <type_traits>
#include <functional>

#include "glm/glm.hpp"
#include "nador/log/ILog.h"
#include "nador/common/Finally.h"
#include "nador/common/Stopwatch.h"

namespace nador
{
    namespace utils
    {
        /*!
         * Gets the next power of the value.
         *
         * \param value     The value.
         * \param power     The power.
         *
         * \return  The next power.
         */
        template <typename T>
        T NextUpPower(const T val, T pow)
        {
            NADOR_ASSERT(pow > 0);
            --pow;
            T temp = 1;
            while (val > temp)
            {
                temp <<= pow;
            }
            return temp;
        }

        /*!
         * Gets the vector size in bytes.
         *
         * \param vec          The vector.
         *
         * \return The size in bytes.
         */
        template <typename T>
        size_t VectorsizeInBytes(const typename std::vector<T>& vec)
        {
            return sizeof(T) * vec.size();
        }

        /*!
         * Checks if std::future is done or not.
         *
         * \param future          The future object.
         *
         * \return      True if done otherwise false.
         */
        template <typename R>
        bool isReadyFuture(const std::future<R>& future)
        {
            if (future.valid())
            {
                return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
            }
            return true;
        }

        /*!
         * Removes whitespaces from the start and from the end of the string.
         *
         * \param s          The string.
         *
         * \return      The cleared string.
         */
        std::string trim(const char* s);

        template <typename ContainerTy, typename ElemTy>
        bool Find(const ContainerTy& container, const ElemTy& elem)
        {
            return std::find(container.begin(), container.end(), elem) != container.end();
        }

        template <typename ContainerTy, typename ElemTy>
        void Remove(ContainerTy& container, const ElemTy& elem)
        {
            container.erase(std::remove(container.begin(), container.end(), elem), container.end());
        }

        template <typename ContainerTy>
        ContainerTy GetAllElementsOnPlace(const ContainerTy container, size_t start, size_t step)
        {
            ContainerTy results;

            for (size_t i = start; i < container.size(); i += step)
            {
                results.push_back(container.at(i));
            }

            return results;
        }

        int32_t CountMatchInRegex(const std::string& s, const std::regex& regex);

        template <typename EngineTy>
        EngineTy& RandomEngine()
        {
            static std::random_device rd;
            static EngineTy           engine(rd());
            return engine;
        }

        void PopBackUtf8(std::string& utf8);

        template <typename T>
        inline std::string StringCast(const T& t)
        {
            std::ostringstream buff;
            buff << t;
            return buff.str();
        }

        template <typename E>
        constexpr auto ToIntegral(E e) -> typename std::underlying_type<E>::type
        {
            return static_cast<typename std::underlying_type<E>::type>(e);
        }

        template <typename ResultT, typename ContainerT, typename Pred>
        constexpr auto Extraxt(const ContainerT& c, Pred pred)
        {
            ResultT retval;
            for (auto const& element : c)
            {
                pred(element, retval);
            }
            return retval;
        }

        template <typename BeforeCallable, typename AfterCallable, typename CallableTy, typename... Args>
        decltype(auto) WrappCallable(BeforeCallable b, AfterCallable a, CallableTy&& c, Args&&... args)
        {
            // calling before callback
            b();

            // setup exit callback
            auto onExit = Finally(std::move(a));

            // invoke function
            return std::invoke(std::forward<CallableTy>(c), std::forward<Args>(args)...);
        }

        template <typename DurationTy, typename CallableTy, typename... Args>
        decltype(auto) MeasureTime(DurationTy& dur_out, CallableTy&& c, Args&&... args)
        {
            Stopwatch sw;

            auto before = [&sw] { sw.Start(); };
            auto after  = [&sw, &dur_out]() mutable { dur_out = sw.Stop<DurationTy>(); };

            return WrappCallable(before, after, std::forward<CallableTy>(c), std::forward<Args>(args)...);
        }

        template <typename ValueT, typename BeginCBT, typename EndCBT>
        struct IterableWrapper
        {
            using iterator       = decltype(std::declval<BeginCBT>()(std::declval<ValueT&>()));
            using const_iterator = decltype(std::declval<BeginCBT>()(std::declval<const ValueT&>()));

            IterableWrapper(ValueT& value, BeginCBT beginCb, EndCBT endCb)
            : _value(value)
            , _beginCb(beginCb)
            , _endCb(endCb)
            {
            }

            iterator       begin() { return _beginCb(_value); }
            const_iterator begin() const { return _beginCb(_value); }
            iterator       end() { return _endCb(_value); }
            const_iterator end() const { return _endCb(_value); }

        private:
            ValueT&  _value;
            BeginCBT _beginCb;
            EndCBT   _endCb;
        };

    } // namespace utils
} // namespace nador

#endif // !__UTILS_UTILS_H__
