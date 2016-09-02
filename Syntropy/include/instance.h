/// \file instance.h
/// \brief This header is part of the syntropy reflection system. It contains methods and classes used to handle object instances.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <type_traits>

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

                virtual std::unique_ptr<IContent> Clone() const noexcept = 0;

                virtual std::unique_ptr<IContent> ConstClone() const noexcept = 0;

            };

            template <typename TContent>
            struct Content : IContent {

                Content(TContent& content);

                virtual const Type& GetType() const noexcept override;

                virtual std::unique_ptr<IContent> Clone() const noexcept override;

                virtual std::unique_ptr<IContent> ConstClone() const noexcept override;

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

    }

}

namespace std {

    void swap(syntropy::reflection::Instance& first, syntropy::reflection::Instance& second) noexcept;

}

namespace syntropy {

    namespace reflection {

        //////////////// INSTANCE ////////////////

        template <typename TContent, typename>
        Instance::Instance(TContent& content) noexcept
            : content_(std::make_unique<Content<TContent>>(content)) {}

        template <typename TContent>
        typename std::remove_reference_t<TContent>* Instance::As() const {

            return (content_ && GetType() == TypeOf<TContent>()) ?
                   &(static_cast<Content<std::remove_reference_t<TContent>>*>(content_.get())->content_.get()) :
                   nullptr;

        }

        //////////////// INSTANCE :: CONTENT ////////////////

        template <typename TContent>
        inline Instance::Content<TContent>::Content(TContent& content)
            : content_(std::ref(content)) {}

        template <typename TContent>
        inline const Type& Instance::Content<TContent>::GetType() const noexcept {

            return TypeOf<TContent>();

        }

        template <typename TContent>
        inline std::unique_ptr<Instance::IContent> Instance::Content<TContent>::Clone() const noexcept {

            return std::make_unique<Content<TContent>>(content_);

        }

        template <typename TContent>
        inline std::unique_ptr<Instance::IContent> Instance::Content<TContent>::ConstClone() const noexcept {

            return std::make_unique<Content<const TContent>>(std::cref(content_));

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