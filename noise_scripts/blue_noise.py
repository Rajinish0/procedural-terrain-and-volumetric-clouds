import sys
import argparse


W, H = 500, 500
parser = argparse.ArgumentParser()
parser.add_argument('-n', '--num_samples', default = 100, help='Number of samples for blue noise')
parser.add_argument('-q', '--quite', action='store_true', default=False, help="don't print this msg")


args = parser.parse_args()
if not args.quite:
	parser.print_help()

NUM_SAMPLES = args.num_samples

print(NUM_SAMPLES)

