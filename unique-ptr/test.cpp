#include "unique_ptr.h"

#include <optional>
#include <concepts>

#include <catch2/catch_test_macros.hpp>

namespace {

struct Counters {
    bool operator==(const Counters&) const = default;

    int ctor{};
    int intctor{};
    int dtor{};
    int method{};
};

struct Fixture {
    Fixture() {
        counters = {};
    }

    static inline Counters counters;

    struct TestClass;
};

struct Fixture::TestClass {
    TestClass() {
        ++counters.ctor;
    }
    explicit TestClass(int) {
        ++counters.intctor;
    }

    ~TestClass() {
        ++counters.dtor;
    }

    TestClass(const TestClass&) = delete;
    TestClass(TestClass&&) = delete;
    TestClass& operator=(const TestClass&) = delete;
    TestClass& operator=(TestClass&&) = delete;

    void Method() {
        ++counters.method;
    }
};

}  // namespace

TEST_CASE_METHOD(Fixture, "Checks") {
    using UPtr = UniquePtr<TestClass>;

    STATIC_CHECK(std::move_constructible<UPtr>);
    STATIC_CHECK(std::assignable_from<UPtr&, UPtr&&>);

    STATIC_CHECK_FALSE(std::copy_constructible<UPtr>);
    STATIC_CHECK_FALSE(std::assignable_from<UPtr&, const UPtr&>);
}

TEST_CASE_METHOD(Fixture, "Empty") {
    UniquePtr<TestClass> ptr;

    REQUIRE(counters == Counters{});
    REQUIRE(ptr.Get() == nullptr);
    REQUIRE(ptr.Release() == nullptr);
    REQUIRE_FALSE(ptr);
}

TEST_CASE_METHOD(Fixture, "Simple") {
    auto* raw_ptr = new TestClass;
    auto opt_ptr = std::make_optional<UniquePtr<TestClass>>(raw_ptr);
    auto& ptr = *opt_ptr;

    REQUIRE(ptr.Get() == raw_ptr);
    REQUIRE(&*ptr == raw_ptr);
    REQUIRE(counters == Counters{1});
    REQUIRE(ptr);

    ptr.Reset();
    REQUIRE(counters == Counters{1, 0, 1});

    ptr.Reset(new TestClass{1});
    REQUIRE(counters == Counters{1, 1, 1});

    ptr.Reset(new TestClass{2});
    REQUIRE(counters == Counters{1, 2, 2});

    ptr->Method();
    REQUIRE(counters == Counters{1, 2, 2, 1});

    opt_ptr.reset();
    REQUIRE(counters == Counters{1, 2, 3, 1});
}

TEST_CASE_METHOD(Fixture, "Move ctor") {
    auto* raw_ptr = new TestClass;
    UniquePtr<TestClass> ptr{raw_ptr};
    auto ptr2 = std::move(ptr);

    REQUIRE(ptr2.Get() == raw_ptr);
    REQUIRE(ptr.Get() == nullptr);
    REQUIRE(ptr2);
    REQUIRE_FALSE(ptr);

    REQUIRE(counters == Counters{1});
}

TEST_CASE_METHOD(Fixture, "Move") {
    UniquePtr<TestClass> ptr{new TestClass};
    UniquePtr<TestClass> ptr2;
    REQUIRE(counters == Counters{1});

    ptr2 = std::move(ptr);
    REQUIRE(counters == Counters{1});

    ptr2 = std::move(ptr);
    REQUIRE(counters == Counters{1, 0, 1});

    ptr.Reset();
    ptr2.Reset();
    REQUIRE(counters == Counters{1, 0, 1});
}

TEST_CASE_METHOD(Fixture, "Move 2") {
    UniquePtr<TestClass> ptr{new TestClass};
    UniquePtr<TestClass> ptr2{new TestClass{2}};
    REQUIRE(counters == Counters{1, 1});

    ptr = std::move(ptr2);
    REQUIRE(counters == Counters{1, 1, 1});

    UniquePtr ptr3(std::move(ptr));
    REQUIRE(counters == Counters{1, 1, 1});

    ptr3.Reset();
    REQUIRE(counters == Counters{1, 1, 2});
}

TEST_CASE_METHOD(Fixture, "Constness") {
    auto* raw_ptr = new TestClass;
    const UniquePtr<TestClass> ptr{raw_ptr};

    REQUIRE(ptr.Get() == raw_ptr);
    REQUIRE(&*ptr == raw_ptr);
    REQUIRE(ptr);

    ptr->Method();
    REQUIRE(counters == Counters{1, 0, 0, 1});
}

TEST_CASE("Inheritance") {
    struct Base {
        virtual ~Base() = default;
        virtual int Foo() {
            return 1;
        };
    };

    struct Derived : Base {
        int Foo() override {
            return 2;
        };
    };

    SECTION("Simple constructor") {
        UniquePtr<Base> ptr{new Derived};
        CHECK(ptr->Foo() == 2);
    }

    SECTION("Move constructor") {
        UniquePtr<Derived> ptr{new Derived};
        UniquePtr<Base> ptr2 = std::move(ptr);
        CHECK(ptr2->Foo() == 2);
    }

    SECTION("Move assignment") {
        UniquePtr<Derived> ptr{new Derived};
        UniquePtr<Base> ptr2;

        ptr2 = std::move(ptr);
        CHECK(ptr2->Foo() == 2);
    }
}
