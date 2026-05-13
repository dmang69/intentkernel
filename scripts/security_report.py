#!/usr/bin/env python3
import csv, sys, os

def main(csv_file="metrics.csv"):
    if not os.path.exists(csv_file):
        print(f"Error: {csv_file} not found")
        return 1
    tests = []
    with open(csv_file) as f:
        for row in csv.DictReader(f):
            tests.append(row)
    print("=" * 60)
    print("INTENTKERNEL SECURITY EVALUATION REPORT")
    print("=" * 60)
    for t in tests:
        rate = float(t['denial_rate'])
        bar = '#' * int(rate / 5)
        print(f"  {t['test_name']:20s} |{bar:<20}| {rate:5.1f}%")
    total_d = sum(int(t['denials']) for t in tests)
    total_t = sum(int(t['total']) for t in tests)
    if total_t > 0:
        print(f"\n  Security Rate: {total_d}/{total_t} = {total_d/total_t*100:.1f}%")
        status = 'PASS' if total_d == total_t else 'FAIL'
        msg = ("0 escapes detected" if total_d == total_t
               else f"{total_t-total_d} escapes detected")
        print(f"  {status}: {msg}")
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv[1] if len(sys.argv) > 1 else "metrics.csv"))
