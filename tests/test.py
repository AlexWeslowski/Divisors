import divisors as d

assert d.__version__ == "0.0.1"
assert d.divisors(2) == {2:1}
combinations = d.Combinations(3293136)
combinations.backtrack(3293136, [])
assert len(combinations) == 676
assert len([x for x in combinations]) == 676
assert len(combinations.to_array()) == 676

