#ifndef ENGINE_BASE_H
#define ENGINE_BASE_H

#include "Ref.h"

namespace Engine
{
    // Pointer wrappers
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    using byte = uint8_t;

    /** A simple wrapper for std::atomic_flag to avoid confusing
        function names usage. The object owning it can still be
        default copyable, but the copied flag is going to be reset.
    */
    struct AtomicFlag
    {
        void SetDirty() { flag.clear(); }
        bool CheckAndResetIfDirty() { return !flag.test_and_set(); }

        explicit AtomicFlag() noexcept { flag.test_and_set(); }
        AtomicFlag(const AtomicFlag&) noexcept {}
        AtomicFlag& operator=(const AtomicFlag&) noexcept { return *this; }
        AtomicFlag(AtomicFlag&&) noexcept {};
        AtomicFlag& operator=(AtomicFlag&&) noexcept { return *this; }

    private:
        std::atomic_flag flag;
    };

    struct Flag
    {
        void SetDirty() noexcept { flag = true; }
        bool CheckAndResetIfDirty() noexcept
        {
            if (flag)
                return !(flag = !flag);
            else
                return false;
        }

        bool IsDirty() const noexcept { return flag; }

    private:
        bool flag = false;
    };
} // namespace Engine

#define BIT(x) (1u << x)

#define hnew new
#define hdelete delete

#endif // ENGINE_BASE_H
