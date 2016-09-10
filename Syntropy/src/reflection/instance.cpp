#include "instance.h"

namespace syntropy {

    namespace reflection {

        //////////////// INSTANCE ////////////////

        Instance::Instance(const Instance& other) noexcept
            : content_(other.content_ ? other.content_->Clone() : nullptr) {}

        Instance::Instance(ConstCloneTag, const Instance& other) noexcept
            : content_(other.content_ ? other.content_->ConstClone() : nullptr) {}

        Instance::Instance(Instance&& other) noexcept
            : content_(std::move(other.content_)) {}

        Instance& Instance::operator=(const Instance& other) noexcept {

            Instance(other).Swap(*this);

            return *this;

        }

        Instance& Instance::operator=(Instance&& other) noexcept {

            Instance(std::move(other)).Swap(*this);

            return *this;

        }

        void Instance::Clear() noexcept {

            content_.reset();

        }

        void Instance::Swap(Instance& other) noexcept {

            std::swap(content_, other.content_);

        }

        Instance::operator bool() const noexcept {

            return !!content_;

        }

        Instance Instance::ToConst() const noexcept {

            return Instance(ConstCloneTag(), *this);

        }

        const Type& Instance::GetType() const noexcept {

            return content_->GetType();

        }

        std::ostream& Instance::operator >> (std::ostream& output_stream) const {

            return content_ ?
                   *content_ >> output_stream :
                   output_stream;                      // Writing nothing to a stream is always a valid operation.

        }

        std::istream& Instance::operator << (std::istream& input_stream) {
            
            return content_ ?
                   *content_ << input_stream :
                   input_stream;                       // Reading nothing from a stream is always a valid operation.

        }

        //////////////// MAKE INSTANCE ////////////////

        Instance MakeInstance(Instance instance) {

            return Instance(instance);

        }

        Instance MakeConstInstance(Instance instance) {

            return instance.ToConst();

        }

    }

}

namespace std {

    void swap(syntropy::reflection::Instance& first, syntropy::reflection::Instance& second) noexcept {

        first.Swap(second);

    }

    std::istream& operator >> (std::istream& input_stream, syntropy::reflection::Instance& instance) {

        return instance << input_stream;
        
    }

    std::ostream& operator << (std::ostream& output_stream, const syntropy::reflection::Instance& instance) {
        
        return instance >> output_stream;
        
    }

}