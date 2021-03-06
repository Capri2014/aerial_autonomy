#include <aerial_autonomy/common/type_map.h>
#include <gtest/gtest.h>

//// \brief Definitions
///  Define any necessary subclasses for tests here
struct BaseClass {
  int i = 0;
};
struct SubClass1 : BaseClass {
  int i = 1;
};
struct SubClass2 : BaseClass {
  int i = 2;
};
///

/// \brief TEST
/// All the tests are defined here
TEST(TypeMapTests, SaveAndRetrieveObject) {
  TypeMap<BaseClass> type_map;
  SubClass1 subclass1;
  SubClass2 subclass2;
  type_map.setObject(subclass1);
  type_map.setObject(subclass2);
  ASSERT_NO_THROW(type_map.getObject<SubClass1>());
  ASSERT_NO_THROW(type_map.getObject<SubClass2>());
  SubClass1 *object1 = type_map.getObject<SubClass1>();
  SubClass2 *object2 = type_map.getObject<SubClass2>();
  object1->i = 1; // Should override both because its the same object;
  object1->i = 2; // Should override both because its the same object;
  ASSERT_EQ(object1->i, subclass1.i);
  ASSERT_EQ(object2->i, subclass2.i);
}

TEST(TypeMapTests, OverrideObject) {
  TypeMap<BaseClass> type_map;
  SubClass1 instance1;
  type_map.setObject(instance1);
  SubClass1 instance2;
  instance2.i = 10;
  type_map.setObject(instance2);
  SubClass1 *object1 = type_map.getObject<SubClass1>();
  ASSERT_EQ(object1->i, instance2.i);
  ASSERT_NE(object1->i, instance1.i);
}
///

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
