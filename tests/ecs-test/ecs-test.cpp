//
// Created by roman Sztergbaum on 30/05/2018.
//

#include <gtest/gtest.h>
#include <entt/signal/dispatcher.hpp>
#include <shiva/reflection/reflection.hpp>
#include <shiva/meta/list.hpp>
#include <shiva/ecs/ecs.hpp>
#include <shiva/ecs/system_manager.hpp>
#include <shiva/ecs/details/system_type_traits.hpp>


template<typename system_type>
class FakeSystem
{
public:
    template<typename T>
    using is_same_system = std::is_same<system_type, T>;

    FakeSystem()
    {
        using is_same_list = meta::list::Transform<shiva::ecs::details::valid_systems_list, is_same_system>;
        static_assert(shiva::ecs::details::is_valid_system_v(is_same_list{}));
    }
};

class fixture_system : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

protected:
    entt::Dispatcher dispatcher_{};
    entt::DefaultRegistry registry_{};
    shiva::ecs::system_manager system_manager_{dispatcher_, registry_};
};

class test_system : public shiva::ecs::system<test_system, shiva::ecs::system_post_update>
{
public:
    reflect_class(test_system);
public:
   test_system(shiva::ecs::dispatcher &dispatcher, shiva::ecs::entity_registry& registry) :
   system(dispatcher, registry)
   {

   }

    void update() noexcept override
    {
       std::cout << "YOY" << std::endl;
    }
private:
};

class another_test_system : public shiva::ecs::system<another_test_system, shiva::ecs::system_pre_update>
{
public:
    reflect_class(another_test_system);
public:
    another_test_system(shiva::ecs::dispatcher &dispatcher, shiva::ecs::entity_registry& registry) :
        system(dispatcher, registry)
    {

    }

    void update() noexcept override
    {
        std::cout << "YOY" << std::endl;
    }
private:
};

TEST(ecs_testing, constructor)
{
    entt::Dispatcher dispatcher{};
    entt::DefaultRegistry registry{};
    shiva::ecs::system_manager manager(dispatcher, registry);
    ASSERT_EQ(1, 1);
}

TEST(ecs_testing, type_traits)
{
    [[maybe_unused]] FakeSystem<shiva::ecs::system_logic_update> s;
}

TEST_F(fixture_system, add_simple_system)
{
    auto system = system_manager_.create_system<test_system>();
    ASSERT_EQ(system_manager_.nb_systems(), 1u);
    ASSERT_TRUE(system_manager_.has_system<test_system>());
    system.update();
}

TEST_F(fixture_system, get_simple_system)
{
    system_manager_.create_system<test_system>();
    [[maybe_unused]] const auto& c_sys = system_manager_.get_system<test_system>();
    [[maybe_unused]] auto& sys = system_manager_.get_system<test_system>();

    system_manager_.get_system<another_test_system>();
}