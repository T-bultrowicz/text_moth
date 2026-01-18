import random
import argparse
from pathlib import Path
import subprocess
import difflib

# Config defaults
MAX_TEXT_ID = 1000
MAX_TEXT_LENGTH = 111
MAX_VITALITY = 1000
MAX_RANGE = 99
MAX_CYCLES = 10

MOTH_KINDS = ["*", "A", "1", "!"]
VALID_CHARS = [chr(c) for c in range(33, 127)]


def generate_text(texts: dict, available_ids: set[int]):
    if not available_ids:
        return None

    text_id = random.choice(tuple(available_ids))
    available_ids.remove(text_id)

    length = random.randint(1, MAX_TEXT_LENGTH)
    texts[text_id] = length
    content = "".join(random.choice(VALID_CHARS) for _ in range(length))
    return f"TEXT {text_id} {content}"


def generate_moth(texts: dict, available_ids: set[int]):
    if not texts:
        return None

    text_id = random.choice(list(texts.keys()))
    position = random.randint(0, texts[text_id] - 1)
    kind = random.choice(MOTH_KINDS)
    vitality = random.randint(1, MAX_VITALITY)
    range_ = random.randint(1, MAX_RANGE)
    return f"MOTH {text_id} {position} {kind} {vitality} {range_}"


def generate_feed(texts: dict, available_ids: set[int]):
    if not texts:
        return None

    text_id = random.choice(list(texts.keys()))
    cycles = random.randint(1, MAX_CYCLES)
    return f"FEED {text_id} {cycles}"


def generate_printm(texts: dict, available_ids: set[int]):
    if not texts:
        return None

    text_id = random.choice(list(texts.keys()))
    return f"PRINTM {text_id}"


def generate_printt(texts: dict, available_ids: set[int]):
    if not texts:
        return None

    text_id = random.choice(list(texts.keys()))
    return f"PRINTT {text_id}"


def generate_delete(texts: dict, available_ids: set[int]):
    if not texts:
        return None

    text_id = random.choice(list(texts.keys()))
    texts.pop(text_id)
    available_ids.add(text_id)
    return f"DELETE {text_id}"


COMMAND_GENERATORS = {
    "TEXT": generate_text,
    "MOTH": generate_moth,
    "FEED": generate_feed,
    "PRINTM": generate_printm,
    "PRINTT": generate_printt,
    "DELETE": generate_delete,
}


def generate_input(lines_count: int) -> str:
    texts: dict[int, int] = {}
    available_ids = set(range(MAX_TEXT_ID + 1))
    lines: list[str] = []

    while len(lines) < lines_count:
        cmd_type = random.choices(
            list(COMMAND_GENERATORS.keys()),
            weights=[3, 3, 2, 1, 1, 1],
            k=1,
        )[0]

        if not texts:
            cmd_type = "TEXT"

        cmd = COMMAND_GENERATORS[cmd_type](texts, available_ids)
        if cmd is not None:
            lines.append(cmd)

    return "\n".join(lines) + "\n"


def run_all(exec_path: str, in_dir: Path, out_dir: Path):
    out_dir.mkdir(parents=True, exist_ok=True)

    for infile in sorted(in_dir.glob("*.in")):
        outfile = out_dir / (infile.stem + ".out")
        print(f"[RUN] {infile.name}")

        with infile.open("r") as fin, outfile.open("w") as fout:
            subprocess.run(
                [exec_path],
                stdin=fin,
                stdout=fout,
                stderr=subprocess.DEVNULL,
                check=False,
            )


def diff_all(out_dir: Path, ref_dir: Path):
    out_files = {p.name: p for p in out_dir.glob("*.out")}
    ref_files = {p.name: p for p in ref_dir.glob("*.out")}

    all_names = sorted(out_files.keys() | ref_files.keys())

    diff_dir = Path("diffs")

    for name in all_names:
        out_file = out_files.get(name)
        ref_file = ref_files.get(name)

        if not out_file or not ref_file:
            print(f"[MISSING] {name}")
            continue

        out_lines = out_file.read_text().splitlines()
        ref_lines = ref_file.read_text().splitlines()

        if out_lines == ref_lines:
            print(f"\033[32m[OK]\033[0m {name}")
            continue

        print(f"\033[31m[DIFF]\033[0m  {name}")
        diff = difflib.unified_diff(
            ref_lines,
            out_lines,
            fromfile=f"to_diff/{name}",
            tofile=f"out/{name}",
            lineterm="",
        )

        diff_dir.mkdir(exist_ok=True)

        diff_path = diff_dir / Path(name).with_suffix(".diff")
        diff_path.write_text("\n".join(diff))
        print(f"Diff written to {diff_path}")


def main():
    parser = argparse.ArgumentParser()
    sub = parser.add_subparsers(dest="mode", required=True)

    # gen mode
    gen = sub.add_parser("gen", help="Generate random .in files")
    gen.add_argument("--lines", type=int, default=100)
    gen.add_argument("--seed", type=int, default=123)
    gen.add_argument("--in-dir", default="in")

    # run mode
    run = sub.add_parser("run", help="Run program on existing .in files")
    run.add_argument("--exec", default="./text_moths")
    run.add_argument("--in-dir", default="in")
    run.add_argument("--out-dir", default="out")

    # diff mode
    diff = sub.add_parser("diff", help="Diff against reference outputs")
    diff.add_argument("--out-dir", default="out")
    diff.add_argument("--ref-dir", default="to_diff")

    args = parser.parse_args()

    if args.mode == "gen":
        random.seed(args.seed)

        in_dir = Path(args.in_dir)
        in_dir.mkdir(parents=True, exist_ok=True)

        data = generate_input(args.lines)
        path = in_dir / f"{args.lines}_{args.seed}.in"
        path.write_text(data)
        print(f"[GEN] {path}")

    elif args.mode == "run":
        run_all(args.exec, Path(args.in_dir), Path(args.out_dir))

    elif args.mode == "diff":
        diff_all(Path(args.out_dir), Path(args.ref_dir))


if __name__ == "__main__":
    main()
