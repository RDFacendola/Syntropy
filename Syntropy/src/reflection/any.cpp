#include "reflection/any.h"

#include "reflection/class.h"

namespace syntropy
{
    namespace reflection
    {

        /************************************************************************/
        /* ANY                                                                  */
        /************************************************************************/

        Any::Any(const Any& other)
            : holder_(other.HasValue() ? other.holder_->Clone() : nullptr)
        {

        }

        Any::Any(Any&& other) noexcept
            : holder_(std::move(other.holder_))
        {
            other.holder_ = nullptr;
        }

        Any& Any::operator=(const Any& other)
        {
            Any(other).Swap(*this);
            return *this;
        }

        Any& Any::operator=(Any&& other) noexcept
        {
            Any(std::move(other)).Swap(*this);
            return *this;
        }

        Any::~Any()
        {
            Reset();
        }

        void Any::Reset() noexcept
        {
            if (holder_)
            {
                delete holder_;
                holder_ = nullptr;
            }
        }

        void Any::Swap(Any& other) noexcept
        {
            std::swap(holder_, other.holder_);
        }

        bool Any::HasValue() const noexcept
        {
            return !!holder_;
        }

        const Type& Any::GetType() const noexcept
        {
            return holder_ ?
                holder_->GetType() :
                TypeOf<void>();
        }

        const type_info& Any::GetTypeInfo() const noexcept
        {
            return holder_ ?
                holder_->GetTypeInfo() :
                typeid(void);
        }

    }
}

namespace std
{

    void swap(syntropy::reflection::Any& first, syntropy::reflection::Any& second) noexcept
    {
        first.Swap(second);
    }

}
