#ifndef __NADOR_NON_COPYABLE_H__
#define __NADOR_NON_COPYABLE_H__

namespace nador
{
    class NonCopyable
    {
    protected:
        /*!
         * NonCopyable default constructor.
         */
        constexpr NonCopyable() = default;

        /*!
         * NonCopyable default destructor.
         */
        ~NonCopyable() = default;

        /*!
         * NonCopyable copy destructor deleted.
         */
        NonCopyable(const NonCopyable&) = delete;

        /*!
         * NonCopyable copy destructor deleted.
         */
        NonCopyable& operator=(const NonCopyable&) = delete;
    };

    class OnlyMoveable
    {
    protected:
        /*!
         * OnlyMoveable default constructor.
         */
        constexpr OnlyMoveable() = default;

        /*!
         * OnlyMoveable default destructor.
         */
        ~OnlyMoveable() = default;

        /*!
         * OnlyMoveable move constructor defaulted.
         */
        OnlyMoveable(OnlyMoveable&&) = default;

        /*!
         * OnlyMoveable move assign operator defaulted.
         */
        OnlyMoveable& operator=(OnlyMoveable&&) = default;

        /*!
         * OnlyMoveable copy destructor deleted.
         */
        OnlyMoveable(const OnlyMoveable&) = delete;

        /*!
         * OnlyMoveable copy destructor deleted.
         */
        OnlyMoveable& operator=(const OnlyMoveable&) = delete;
    };
    
} // namespace nador

#endif // !__NADOR_NON_COPYABLE_H__
