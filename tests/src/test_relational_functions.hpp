// Relational Functions
//
test_true(all(notEqual(lessThan(a, b), greaterThanEqual(a, b))),	"all lessThan / greaterThanEqual (exclusivity)");
test_true(all(notEqual(lessThanEqual(a, b), greaterThan(a, b))),	"all lessThanEqual / greaterThan (exclusivity)");
// TODO any all equal notEqual (and another test for lessThen etc.)
