

#include <iostream>
#include <iomanip>

#include "syntropy/application/command_line.h"

#include "syntropy/unit_test/test_runner.h"

#include "syntropy/time/timer.h"
#include "syntropy/math/vector.h"
#include "syntropy/math/random.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/math/rotation.h"

#include "syntropy/containers/stream_vector.h"
#include "syntropy/containers/stream_vector_view.h"

void InvalidateCache()
{
    using namespace syntropy;

    auto rng = syntropy::Random{};

    auto size = 1_GiBytes;
    auto max = char{ -127 };

    auto block = reinterpret_cast<char*>(malloc(std::size_t(size)));

    for (auto index = 0; index < std::size_t(size); ++index)
    {
        block[index] ^= static_cast<char>(rng.Range(0, 255));

        max = std::max(max, block[index]);
    }

    delete block;

    std::cout << "Cache invalidated (" << max << ")\n";
}

int main(int argc, char **argv)
 {
     struct Element
     {
         syntropy::Float3 position_;
         syntropy::Float3 force_;
         syntropy::Float3 velocity_;
         float mass_;
     };

     auto elements = std::vector<Element>{};
     auto count = std::size_t{};
     auto iterations = std::size_t{};

     auto delta_time = 0.125f;

     std::cout << "How many elements? ";
     std::cin >> count;

     std::cout << "How many iterations? ";
     std::cin >> iterations;

     // Generate random elements.

     {
         auto rng = syntropy::Random{};

         elements.reserve(count);

         for (auto element_index = 0; element_index < count; ++element_index)
         {
             auto x = rng.Range(-10000.0f, 10000.0f);
             auto y = rng.Range(-10000.0f, 10000.0f);
             auto z = rng.Range(-10000.0f, 10000.0f);

             auto fx = rng.Range(-10.0f, 10.0f);
             auto fy = rng.Range(-10.0f, 10.0f);
             auto fz = rng.Range(-10.0f, 10.0f);

             auto velocity = syntropy::Float3{ 0.0f };

             auto mass = rng.Range(1.0f, 100.0f);

             auto position = syntropy::Float3{ x,y,z };
             auto force = syntropy::Float3{ x,y,z };

             force = Normalize(force);

             elements.emplace_back(Element{ position, force, velocity, mass });
         }
     }

     // Structures.

     class Object
     {
     public:

         std::size_t uuid_;
         std::string name_;
         std::vector<std::string> tag_;

         virtual void Execute(float delta_time) = 0;
     };

     class PositionalObject : public Object
     {
     public:

         syntropy::Float3 position_;
         syntropy::Rotation rotation_;
         syntropy::Float3 scale_;
         int mobility_;
         bool b_dirty_;
     };

     class RigidBodyObject : public PositionalObject
     {
     public:

         syntropy::Float3 force_;
         syntropy::Float3 velocity_;
         float mass_;
         float inv_mass_;
         syntropy::Float3 min_bounds_;
         syntropy::Float3 max_bounds_;
         float bounciness_;
         int physical_material_index_;

         virtual void Execute(float delta_time)
         {
             auto acceleration = force_ / mass_;

             velocity_ += acceleration * delta_time;
             position_ += velocity_ * delta_time;
         }
     };

     auto objects = std::vector<Object*>{};

     objects.reserve(count);

     {
         auto rng = syntropy::Random{};

         for (auto index = 0; index < count; ++index)
         {
            objects.emplace_back(new RigidBodyObject{});
            auto leak = reinterpret_cast<char*>(malloc(rng.Range(1, 1024)));            // Random allocation.

            *leak = static_cast<char>(rng.Range(0, 255));
         }

         rng.Shuffle(begin(objects), end(objects));
     }

     struct Interleaved
     {
         syntropy::Float3 position_;
         syntropy::Float3 force_;
         syntropy::Float3 velocity_;
         float mass_;
     };

     auto interleaved = std::vector<Interleaved>{};

     interleaved.reserve(count);

     auto forces = std::vector<syntropy::Float3>{};
     auto masses = std::vector<float>{};
     auto positions = std::vector<syntropy::Float3>{};
     auto velocities = std::vector<syntropy::Float3>{};

     positions.reserve(count);
     forces.reserve(count);
     velocities.reserve(count);
     masses.reserve(count);

     auto object_index = 0;

    for (auto&& element : elements)
    {
        auto object = static_cast<RigidBodyObject*>(objects[object_index++]);

        object->position_ = element.position_;
        object->force_ = element.force_;
        object->velocity_ = element.velocity_;
        object->mass_ = element.mass_;

        interleaved.emplace_back(Interleaved{ element.position_, element.force_, element.velocity_, element.mass_ });

        positions.emplace_back(element.position_);
        forces.emplace_back(element.force_);
        velocities.emplace_back(element.velocity_);
        masses.emplace_back(element.mass_);
    }

     // OOP test.

     {
         std::cout << "OOP test running...";

         InvalidateCache();

         syntropy::Timer timer{};

         for (auto iteration = 0; iteration < iterations; ++iteration)
         {
             for (auto&& object : objects)
             {
                 object->Execute(delta_time);
             }
         }

         auto time = timer();

         std::cout << time.count() << "ms\n";
     }


     // Interleaved test.

    {
        std::cout << "Interleaved test running...";

        InvalidateCache();

        syntropy::Timer timer{};

        for (auto iteration = 0; iteration < iterations; ++iteration)
        {
            for (auto&& element : interleaved)
            {
                auto acceleration = element.force_ / element.mass_;

                element.velocity_ += acceleration * delta_time;
                element.position_ += element.velocity_ * delta_time;
            }
        }

        auto time = timer();

        std::cout << time.count() << "ms\n";
    }

     // Parallel test.

     {
         std::cout << "Parallel test running...";

         InvalidateCache();

         syntropy::Timer timer{};

         for (auto iteration = 0; iteration < iterations; ++iteration)
         {
             for(auto element_index = 0; element_index < count; ++element_index)
             {
                 auto acceleration = forces[element_index] / masses[element_index];

                 velocities[element_index] += acceleration * delta_time;
                 positions[element_index] += velocities[element_index] * delta_time;
             }
         }

         auto time = timer();

         std::cout << time.count() << "ms\n";
     }

     // Test.

     {
         auto same = std::size_t{};

         for (auto element_index = 0; element_index < count; ++element_index)
         {
             if (Distance(positions[element_index], interleaved[element_index].position_) < 1.0f &&
                 Distance(static_cast<RigidBodyObject*>(objects[element_index])->position_, positions[element_index]) < 1.0f)
             {
                 ++same;
             }
         }

         std::cout << "Same position: " << same << " Different position: " << (count - same) << "\n";

         system("pause");
     }

     syntropy::CommandLine command_line(argc, argv);
 
     auto&& test_runner = syntropy::TestRunner::GetInstance();
 
     auto on_started_listener = test_runner.OnStarted().Subscribe([](auto& /*sender*/, auto& /*args*/)
     {
         std::cout << "\nRunning unit tests:\n\n";
     });
 
     auto on_test_suite_started_listener = test_runner.OnTestSuiteStarted().Subscribe([](auto& /*sender*/, auto& args)
     {
         std::cout << "   Testing suite '" << args.test_suite_.GetName() << "'\n";
     });
 
     auto on_test_case_started_listener = test_runner.OnTestCaseStarted().Subscribe([](auto& /*sender*/, auto& args)
     {
         std::cout << "      Testing case '" << args.test_case_.GetName() << "'\n";
     });
 
     auto on_test_case_message_listener = test_runner.OnTestCaseMessageNotified().Subscribe([](auto& /*sender*/, auto& args)
     {
         std::cout << "         " << "             " << args.message_ << "\n";
     });
 
     auto on_test_case_result_listener = test_runner.OnTestCaseResultNotified().Subscribe([](auto& /*sender*/, auto& args)
     {
         std::cout << "         " << std::setw(10) << args.result_ << " : " << args.message_ << "\n";
     });
 
     auto on_test_case_finished_listener = test_runner.OnTestCaseFinished().Subscribe([](auto& /*sender*/, auto& args)
     {
         std::cout << "      Test case result: " << args.result_ << "\n";
     });
 
     auto on_test_suite_finished_listener = test_runner.OnTestSuiteFinished().Subscribe([](auto& /*sender*/, auto& args)
     {
         std::cout << "   Test suite result: " << args.result_ << "\n\n";
     });
 
     auto on_finished_listener = test_runner.OnFinished().Subscribe([](auto& /*sender*/, auto& args)
     {
         std::cout << "Result: " << args.result_ << "\n";
     });
 
     test_runner.Run("");
 
     system("pause");
 
 }
 