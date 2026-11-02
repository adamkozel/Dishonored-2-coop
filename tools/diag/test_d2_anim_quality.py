"""Tests for d2_anim_quality.py, with its mutation twins (every check must go red on a broken twin).

Run: python -I tools/diag/test_d2_anim_quality.py
Prints ALL OK when the tests pass and every twin is caught.
"""
import os
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import d2_anim_quality as aq  # noqa: E402

TWINS = ["skate_ignores_planted", "no_jitter", "loose_snap", "no_rubber"]


def sample(ms, root, toes):
    return {"ms": ms, "root": list(root), "yaw": 0,
            "feet": [{"ankle": 21 + 4 * f, "points": [{"joint": 22 + 4 * f, "at": list(t)}]} for f, t in enumerate(toes)]}


def walk(n=60, dt=40, speed=1.5, slide=0.0):
    """A clean walk: the root moves steadily; each toe alternates planted (still, or sliding by `slide` m/s) and
    swinging (lifted 10 cm and moving forward)."""
    rows = []
    feet = [[0.0, 0.1, 0.0], [0.0, -0.1, 0.0]]
    for i in range(n):
        t = i * dt
        x = speed * t / 1000.0
        for f in range(2):
            phase = (i // 8 + f) % 2
            if phase == 0:
                feet[f][0] += slide * dt / 1000.0
                feet[f][2] = 0.0
            else:
                feet[f][0] += 2 * speed * dt / 1000.0
                feet[f][2] = 0.10
        rows.append(sample(t, (x, 0, 0), [tuple(feet[0]), tuple(feet[1])]))
    return rows


class Quality(unittest.TestCase):
    def test_clean_walk_passes(self):
        r = aq.analyse(walk())
        self.assertEqual(r["status"], "PASS", r)
        self.assertGreater(r["contacts"], 0)

    def test_foot_skate_is_caught(self):
        r = aq.analyse(walk(slide=1.5))
        self.assertIn("footSkateP95", r["failures"])

    def test_jitter_is_caught(self):
        rows = [sample(i * 40, (0.05 * (i % 2), 0, 0), [(0, 0.1, 0), (0, -0.1, 0)]) for i in range(40)]
        r = aq.analyse(rows)
        self.assertIn("jitterEpisodes", r["failures"])

    def test_snap_is_caught(self):
        rows = walk()
        for k in range(30, len(rows)):
            rows[k]["root"][0] += 3.0
        r = aq.analyse(rows)
        self.assertIn("snaps", r["failures"])

    def test_rubber_band_is_caught(self):
        rows = [sample(i * 40, (0, 0, 0), [(0, 0.1, 0), (0, -0.1, 0)]) for i in range(30)]
        rows[10]["root"] = [2.5, 0, 0]
        r = aq.analyse(rows)
        self.assertIn("rubberBands", r["failures"])

    def test_too_few_samples_skip(self):
        self.assertEqual(aq.analyse(walk(n=5))["status"], "SKIP")


def run_twins():
    caught = 0
    for twin in TWINS:
        env = dict(os.environ, COOP_MUT=twin)
        p = subprocess.run([sys.executable, "-I", os.path.abspath(__file__), "--single"], env=env, capture_output=True, text=True)
        if p.returncode != 0:
            caught += 1
        else:
            print("TWIN NOT CAUGHT:", twin)
    return caught


if __name__ == "__main__":
    if "--single" in sys.argv:
        sys.argv.remove("--single")
        res = unittest.main(exit=False, verbosity=0).result
        sys.exit(0 if res.wasSuccessful() else 1)
    res = unittest.main(exit=False, verbosity=0).result
    if not res.wasSuccessful():
        sys.exit(1)
    caught = run_twins()
    print("%d tests, %d twins caught.%s" % (res.testsRun, caught, " ALL OK" if caught == len(TWINS) else ""))
    sys.exit(0 if caught == len(TWINS) else 1)
