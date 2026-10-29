"""Tests for d2_world_diff.py, with its mutation twins (coop-anything: every check must go red on a broken twin).

Run: python -I tools/diag/test_d2_world_diff.py
It runs the tests on the real code, then once per twin in COOP_MUT with the expectation that at least one test fails.
Prints ALL OK when the tests pass and every twin is caught.
"""
import os
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import d2_world_diff as wd  # noqa: E402

TWINS = ["no_stability_gate", "presence_only", "zero_compared_passes", "no_generated_grouping"]


def snap(cats, map_="m"):
    return {"map": map_, "categories": cats}


class Diff(unittest.TestCase):
    def test_equal_worlds(self):
        h = [snap({"doors": {"a": [0, 0]}})] * 3
        r = wd.compare(h, list(h))
        self.assertEqual(r["status"], "EQUAL")
        self.assertEqual(r["comparedKeys"], 1)

    def test_changed_value_is_named(self):
        h = [snap({"usables": {"dial_1": [7, 1]}})] * 3
        c = [snap({"usables": {"dial_1": [1, 1]}})] * 3
        r = wd.compare(h, c)
        self.assertEqual(r["status"], "DIFFERENT")
        d = r["categories"]["usables"]["differences"]
        self.assertEqual([x["key"] for x in d], ["dial_1"])
        self.assertEqual(d[0]["host"], [7, 1])

    def test_unstable_key_is_not_compared(self):
        h = [snap({"doors": {"a": [0, 0], "b": [0, 0]}}), snap({"doors": {"a": [0, 0], "b": [2, 2]}}),
             snap({"doors": {"a": [0, 0], "b": [2, 2]}})]
        c = [snap({"doors": {"a": [0, 0], "b": [0, 0]}})] * 3
        r = wd.compare(h, c)
        self.assertEqual(r["status"], "EQUAL")
        self.assertEqual(r["categories"]["doors"]["unstable"], 1)

    def test_per_player_and_per_character_do_not_fail(self):
        h = [snap({"pickups": {"x": [0, 0]}, "usables": {"note_corvo": [0, 1], "lever": [1, 1]}})] * 3
        c = [snap({"pickups": {}, "usables": {"note_emily": [0, 1], "lever": [1, 1]}})] * 3
        r = wd.compare(h, c)
        self.assertEqual(r["status"], "EQUAL")
        self.assertEqual(len(r["categories"]["pickups"]["differences"]), 1)

    def test_generated_numbers_are_grouped(self):
        h = [snap({"pickups": {}, "usables": {"rope_tie_01_12377": [0, 0], "lever": [1, 1]}})] * 3
        c = [snap({"pickups": {}, "usables": {"rope_tie_01_12380": [0, 0], "lever": [1, 1]}})] * 3
        r = wd.compare(h, c)
        self.assertEqual(r["status"], "EQUAL")

    def test_different_maps_skip(self):
        r = wd.compare([snap({"doors": {"a": 1}}, "m1")], [snap({"doors": {"a": 1}}, "m2")])
        self.assertEqual(r["status"], "SKIP")

    def test_nothing_compared_is_skip_not_pass(self):
        r = wd.compare([snap({"doors": {}})], [snap({"doors": {}})])
        self.assertEqual(r["status"], "SKIP")


def run_suite():
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(Diff)
    return unittest.TextTestRunner(stream=open(os.devnull, "w"), verbosity=0).run(suite).wasSuccessful()


def main():
    if os.environ.get("COOP_MUT"):
        sys.exit(0 if run_suite() else 1)
    if not run_suite():
        unittest.main(argv=[sys.argv[0]], exit=True)
    missed = []
    for twin in TWINS:
        env = dict(os.environ, COOP_MUT=twin)
        if subprocess.run([sys.executable, "-I", os.path.abspath(__file__)], env=env).returncode == 0:
            missed.append(twin)
    if missed:
        print("TWINS MISSED:", ", ".join(missed))
        sys.exit(1)
    tests = len([k for k in Diff.__dict__ if k.startswith("test_")])
    print("%d tests, %d twins caught. ALL OK" % (tests, len(TWINS)))


if __name__ == "__main__":
    main()
