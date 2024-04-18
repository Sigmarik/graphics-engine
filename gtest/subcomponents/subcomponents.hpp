/**
 * @file subcomponents.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Subcomponent tests
 * @version 0.1
 * @date 2024-04-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <logics/scene_component.h>

struct BaseComponent : public SceneComponent {};

struct DerivedComponent : public BaseComponent {};

struct TrickyConstructor : public SceneComponent {
    TrickyConstructor(void*) : id(0xDEADBEEF){};

    unsigned long long id = 0;
};

TEST(Subcomponent, Sharing) {
    Subcomponent<BaseComponent> component;
    Subcomponent<BaseComponent> other_component(component);

    EXPECT_EQ(static_cast<void*>(&*component),
              static_cast<void*>(&*other_component));
}

TEST(Subcomponent, Derivation) {
    Subcomponent<DerivedComponent> component;
    Subcomponent<BaseComponent> other_component(component);

    EXPECT_EQ(static_cast<void*>(&*component),
              static_cast<void*>(&*other_component));
}

TEST(Subcomponent, Construction) {
    Subcomponent<TrickyConstructor> component{nullptr};
    Subcomponent<SceneComponent> other_component(component);

    EXPECT_EQ(component->id, 0xDEADBEEF);
    EXPECT_EQ(static_cast<void*>(&*component),
              static_cast<void*>(&*other_component));
}

TEST(Subcomponent, DelayedConstruction) {
    Subcomponent<TrickyConstructor> component{SubcomponentNone};

    EXPECT_EXIT(component->id = 0, ::testing::KilledBySignal(SIGSEGV), ".*");

    component = Subcomponent<TrickyConstructor>(nullptr);

    EXPECT_EQ(component->id, 0xDEADBEEF);
}

TEST(WeakSubcomponent, FromStrong) {
    Subcomponent<BaseComponent> strong{};
    WeakSubcomponent<BaseComponent> weak(strong);

    EXPECT_FALSE(weak.expired());
    EXPECT_EQ(static_cast<void*>(&*strong), static_cast<void*>(&*weak.lock()));
}

TEST(WeakSubcomponent, FromStrongDerived) {
    Subcomponent<DerivedComponent> strong{};
    WeakSubcomponent<BaseComponent> weak(strong);

    EXPECT_FALSE(weak.expired());
    EXPECT_EQ(static_cast<void*>(&*strong), static_cast<void*>(&*weak.lock()));
}

TEST(WeakSubcomponent, FromWeak) {
    Subcomponent<BaseComponent> strong;
    WeakSubcomponent<BaseComponent> weak_base(strong);
    WeakSubcomponent<BaseComponent> weak(weak_base);

    EXPECT_FALSE(weak.expired());
    EXPECT_EQ(static_cast<void*>(&*strong), static_cast<void*>(&*weak.lock()));
}

TEST(WeakSubcomponent, FromWeakDerived) {
    Subcomponent<DerivedComponent> strong;
    WeakSubcomponent<DerivedComponent> weak_base(strong);
    WeakSubcomponent<BaseComponent> weak(weak_base);

    EXPECT_FALSE(weak.expired());
    EXPECT_EQ(static_cast<void*>(&*strong), static_cast<void*>(&*weak.lock()));
}

TEST(WeakSubcomponent, Lifetime) {
    WeakSubcomponent<BaseComponent> weak;

    EXPECT_TRUE(weak.expired());

    {
        Subcomponent<BaseComponent> strong;
        EXPECT_TRUE(weak.expired());

        weak = strong;

        EXPECT_FALSE(weak.expired());
    }

    EXPECT_TRUE(weak.expired());
}
