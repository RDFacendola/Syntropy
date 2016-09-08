/// \file instance.h
/// \brief This header is part of the syntropy reflection system. It contains methods and classes used to handle object instances.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <type_traits>
#include <iostream>

#include "type.h"

namespace syntropy {

    namespace reflection {

        /// Wraps a reference to any object in a copy, assignable object.
        /// Based on both std::any and std::reference_wrapper specs.
        /// \author Raffaele D. Facendola - September 2016
        class Instance {

        public:

            /// \brief Create a reference to a null object.
            constexpr Instance() noexcept = default;

            /// \brief Store a reference to content.
            /// This overload does not participate in overload resolution if TContent is Instance.
            template <typename TContent, typename = std::enable_if_t<!std::is_same<TContent, Instance>::value>>
            Instance(TContent& content) noexcept;

            /// \brief Construction from temporary object is not allowed.
            template <typename TContent>
            Instance(const TContent&& content) = delete;
            
            /// \brief Copy constructor.
            Instance(const Instance& other) noexcept;

            /// \brief Move constructor.
            Instance(Instance&& other) noexcept;

            /// \brief Default destructor.
            ~Instance() = default;

            /// \brief Copy assignment operator. Drops the current reference and stores a new one.
            Instance& operator=(const Instance& other) noexcept;
            
            /// \brief Move assignment operator. Drops the current reference and stores a new one.
            Instance& operator=(Instance&& other) noexcept;

            /// \brief Assign an unspecified value to the instance referenced by this object.
            /// \param other Reference to the actual object to copy.
            /// \return Returns true if the object to copy could be assigned to the instance referenced by this object, returns false otherwise.
            template <typename TOther>
            bool Assign(const TOther& other);

            template <typename TOther>
            bool Interpret(const TOther& other);
            
            bool ReadFromStream(std::istream& input_stream);

            bool WriteToStream(std::ostream& output_stream) const;

            /// \brief Check whether the instance contains a reference to an actual object or not.
            /// \return Returns true if the instance contains a reference to an actual object, returns false otherwise.
            operator bool() const noexcept;

            /// \brief Get the constant version of this instance's content.
            Instance ToConst() const noexcept;

            /// \brief Drops the current reference.
            void Clear() noexcept;

            /// \brief Swaps the content of this object with another one's.
            void Swap(Instance& other) noexcept;
            
            const Type& GetType() const noexcept;

            template <typename TContent>
            typename std::remove_reference_t<TContent>* As() const;
            
        private:

            struct ConstCloneTag {};

            struct IContent {

                virtual ~IContent() = default;

                virtual const Type& GetType() const noexcept = 0;

                virtual bool Assign(Instance other) = 0;

                virtual std::unique_ptr<IContent> Clone() const noexcept = 0;

                virtual std::unique_ptr<IContent> ConstClone() const noexcept = 0;

                virtual bool ReadFromStream(std::istream& input_stream) = 0;

                virtual bool WriteToStream(std::ostream& output_stream) const = 0;

            };

            template <typename TContent>
            struct Content : IContent {

                Content(TContent& content);

                virtual const Type& GetType() const noexcept override;

                virtual bool Assign(Instance other) override;

                virtual std::unique_ptr<IContent> Clone() const noexcept override;

                virtual std::unique_ptr<IContent> ConstClone() const noexcept override;

                virtual bool ReadFromStream(std::istream& input_stream) override;

                virtual bool WriteToStream(std::ostream& output_stream) const override;

                std::reference_wrapper<TContent> content_;

            };

            /// \brief Copy constructor to const.
            Instance(ConstCloneTag, const Instance& other) noexcept;

            std::unique_ptr<IContent> content_;

        };

        /// This overload does not participate in overload resolution if TContent is Instance.
        template <typename TContent>
        Instance MakeInstance(TContent& content);

        Instance MakeInstance(Instance instance);

        /// This overload does not participate in overload resolution if TContent is Instance.
        template <typename TContent>
        Instance MakeInstance(const TContent&& content) = delete;

        /// This overload does not participate in overload resolution if TContent is Instance.
        template <typename TContent>
        Instance MakeConstInstance(const TContent& instance);

        Instance MakeConstInstance(Instance instance);

        /// This overload does not participate in overload resolution if TContent is Instance.
        template <typename TContent>
        Instance MakeConstInstance(const TContent&& content) = delete;

        template <typename TClass, typename = void>
        struct instantiate {

            Instance operator()() const noexcept {

                return Instance();

            }

        };

        template <typename TClass>
        struct instantiate<TClass, typename std::enable_if_t<std::is_default_constructible_v<TClass>>> {

            Instance operator()() const noexcept {

                return MakeInstance(*new TClass());

            }

        };

        template <typename TAssignee, typename TValue, typename = void>
        struct assign {

            bool operator()(TAssignee& /*assignee*/, const TValue& /*value*/) {
            
                return false;
            
            }

        };

        template <typename TAssignee, typename TValue>
        struct assign<TAssignee, TValue, typename std::enable_if_t<is_assignable_v<TAssignee, TValue>>> {

            bool operator()(TAssignee& assignee, const TValue& value) {

                assignee = value;

                return true;

            }

        };

        template <typename TType, typename = void>
        struct stream_extract {

            bool operator()(std::istream& /*input_stream*/, TType& /*destination*/) {

                return false;

            }

        };

        template <typename TType>
        struct stream_extract<TType, typename std::enable_if_t<is_stream_extractable_v<std::istream, TType>>> {

            bool operator()(std::istream& input_stream, TType& destination) {

                input_stream >> destination;

                return true;

            }

        };

        template <typename TType, typename = void>
        struct stream_insert {

            bool operator()(std::ostream& /*output_stream*/, const TType& /*source*/) {

                return false;

            }

        };

        template <typename TType>
        struct stream_insert<TType, typename std::enable_if_t<is_stream_insertable_v<std::ostream, TType>>> {

            bool operator()(std::ostream& output_stream, const TType& source) {

                output_stream << source;

                return true;

            }

        };
        
    }

}

namespace std {

    void swap(syntropy::reflection::Instance& first, syntropy::reflection::Instance& second) noexcept;

    std::istream& operator >> (std::istream &in, syntropy::reflection::Instance& instance);

    std::ostream& operator << (std::ostream &out, const syntropy::reflection::Instance& instance);

}

namespace syntropy {

    namespace reflection {

        //////////////// INSTANCE ////////////////

        template <typename TContent, typename>
        Instance::Instance(TContent& content) noexcept
            : content_(std::make_unique<Content<TContent>>(content)) {}

        template <typename TOther>
        bool Instance::Assign(const TOther& other) {

            return content_ &&
                   content_->Assign(MakeConstInstance(other));

        }
        
        template <typename TOther>
        bool Instance::Interpret(const TOther& other) {

            if (!content_) {

                return false;

            }

            // TODO: use a more efficient stream as the stringstream will need to access the locale of the current machine in order to determine fractional separators and other stuffs.

            std::stringstream stream;
            
            return stream_insert<TOther>()(stream, other) &&
                   ReadFromStream(stream);

        }

        template <typename TContent>
        typename std::remove_reference_t<TContent>* Instance::As() const {

            return (content_ && GetType() == TypeOf<TContent>()) ?
                   &(static_cast<Content<std::remove_reference_t<TContent>>*>(content_.get())->content_.get()) :
                   nullptr;

        }

        //////////////// INSTANCE :: CONTENT ////////////////

        template <typename TContent>
        Instance::Content<TContent>::Content(TContent& content)
            : content_(std::ref(content)) {}

        template <typename TContent>
        const Type& Instance::Content<TContent>::GetType() const noexcept {

            return TypeOf<TContent>();

        }

        template <typename TContent>
        bool Instance::Content<TContent>::Assign(Instance other) {

            // TODO: determine the overload of the assignment operator or eventually some constructor to perform a copy-assignment.

            auto other_value = other.As<const TContent>();

            if (other_value) {
                
                assign<TContent, TContent>()(content_.get(), *other_value);

            }

            return !!other_value;

        }
        
        template <typename TContent>
        std::unique_ptr<Instance::IContent> Instance::Content<TContent>::Clone() const noexcept {

            return std::make_unique<Content<TContent>>(content_);

        }

        template <typename TContent>
        std::unique_ptr<Instance::IContent> Instance::Content<TContent>::ConstClone() const noexcept {

            return std::make_unique<Content<const TContent>>(std::cref(content_));

        }

        template <typename TContent>
        bool Instance::Content<TContent>::ReadFromStream(std::istream& input_stream) {

            return stream_extract<TContent>()(input_stream, content_.get());

        }

        template <typename TContent>
        bool Instance::Content<TContent>::WriteToStream(std::ostream& output_stream) const {

            return stream_insert<TContent>()(output_stream, content_.get());

        }

        //////////////// MAKE INSTANCE / MAKE CONST INSTANCE ////////////////

        template <typename TContent>
        Instance MakeInstance(TContent& content) {

            return Instance(content);

        }

        template <typename TContent>
        Instance MakeConstInstance(const TContent& content) {

            return Instance(content);

        }

    }

}