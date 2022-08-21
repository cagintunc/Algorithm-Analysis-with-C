"""
A script for automatically grading assignments.
This is the student copy which is slightly different than the instructor copy.
version 26.04.2022 by Ersin Çine
"""

import os
import filecmp
import shutil
import subprocess
import re

def show_warning():
    def print_box(text, title="", width=60):
        words = text.split(" ")
        effective_width = width - 4
        lines = [words[0]]
        for word in words[1:]:
            if len(lines[-1]) + 1 + len(word) <= effective_width:
                lines[-1] += " " + word
            else:
                lines.append(word)

        empty_line = "│ " + " " * effective_width + " │"
        print()
        if title:
            left = (width - len(title)) // 2
            right = width - len(title) - left
            print("┌" + "─" * (left - 2), title, "─" * (right - 2) + "┐")
        else:
            print("┌" + "─" * (width - 2) + "┐")
        print(empty_line)
        for line in lines:
            total = effective_width - len(line)
            left = total // 2
            right = total - left
            centered_line = " " * left + line + " " * right
            print("│ " + centered_line + " │")
        print(empty_line)
        print("└" + "─" * (width - 2) + "┘")
        print()

    text = "This script will copy exercise1.cc, exercise2.cc and exercise3.cc from the corresponding directories to the evaluation directory. " \
           "Make sure that you did not modify the header files! The original header files will be used for the evaluation."
    print_box(text, title="WARNING")


def check_warning():
    def check_whether_modified(original, maybe_modified):
        if filecmp.cmp(original, maybe_modified):
            print(f"Good news: It seems that you have not modified the file {maybe_modified}.")
        else:
            print(f"Warning! It seems that you have modified the file {maybe_modified}. "
                  f"We will ignore that file and evaluate your program using the original header file. "
                  f"If you are on Linux, you can execute the following command to see the differences between two files: diff {original} {maybe_modified}")

    original = os.path.dirname(os.path.realpath(__file__)) + os.path.sep + "exercise1_utils.hpp"
    maybe_modified = os.path.dirname(os.path.dirname(os.path.realpath(__file__))) + os.path.sep + "exercise1" + os.path.sep + "src" + os.path.sep + "exercise1_utils.hpp"
    if os.path.exists(maybe_modified):
        check_whether_modified(original, maybe_modified)

    original = os.path.dirname(os.path.realpath(__file__)) + os.path.sep + "exercise2_utils.hpp"
    maybe_modified = os.path.dirname(os.path.dirname(os.path.realpath(__file__))) + os.path.sep + "exercise2" + os.path.sep + "src" + os.path.sep + "exercise2_utils.hpp"
    if os.path.exists(maybe_modified):
        check_whether_modified(original, maybe_modified)


def show_structure():
    def generate_tree_representation(tree, depth=0, is_last_child=True):

        def is_internal_node(subtree):
            return isinstance(subtree, tuple)

        def is_leaf(subtree):
            return isinstance(subtree, str)

        def get_branch_prefix():
            if depth == 0:
                return ""
            if is_last_child:
                return "└── "
            return "├── "

        if is_internal_node(tree):
            assert len(tree) == 2
            assert isinstance(tree[0], str)
            assert isinstance(tree[1], list)
        else:
            assert is_leaf(tree)

        # There are internal nodes and leaves.
        # e.g. directories are internal nodes, files are leaves.
        # or, operators are internal nodes, literals are leaves.

        # A leaf is str.
        # An internal node with three children is (str, [x, y, z]) where x is an internal node or leaf.

        lines = []
        if is_internal_node(tree):
            label = tree[0]
            children = tree[1]
            lines.append(("internal", get_branch_prefix(), label))
            prefix = ("    " if is_last_child else "│   ") if depth > 0 else ""
            for not_last_child in children[:-1]:
                lines.extend([
                    (line[0], prefix + line[1], line[2])
                    for line in
                    generate_tree_representation(not_last_child,
                                                 depth=depth + 1,
                                                 is_last_child=False)

                ]
                )
            last_child = children[-1]
            lines.extend([
                (line[0], prefix + line[1], line[2])
                for line in
                generate_tree_representation(last_child,
                                             depth=depth + 1,
                                             is_last_child=True)
            ]
            )
        else:
            # tree is a leaf
            label = tree
            lines.append(("leaf", get_branch_prefix(), label))

        return lines

    def print_tree(tree, line_function=lambda x: x, internal_label_function=lambda x: x, leaf_label_function=lambda x: x):
        lines = generate_tree_representation(tree)
        for node_type, branch_prefix, label in lines:
            label_function = leaf_label_function if node_type == "leaf" else internal_label_function
            print(line_function(branch_prefix + label_function(label)))

    class Tree:

        def __init__(self, root_name):
            self.root_name = root_name
            self.subtrees = []

        def add(self, *node_names):
            subtrees = [Tree(node_name) for node_name in node_names]
            self.subtrees.extend(subtrees)
            if len(node_names) == 1:
                return subtrees[0]
            return subtrees

        def compile(self):
            if len(self.subtrees) > 0:
                return self.root_name, [subtree.compile() for subtree in self.subtrees]
            return self.root_name

    """
    required_dir_structure = ("hw02", [("evaluation", ["grader.py", "exercise1_utils.hpp", "exercise2_utils.hpp", "exercise1_tests", "exercise2_tests", "exercise3_tests"]),
                                       ("exercise1", [("src", ["exercise1.cc"])]),
                                       ("exercise2", [("src", ["exercise2.cc"])]),
                                       ("exercise3", [("src", ["exercise3.cc"])])])
    """
    hw02 = Tree("hw02")
    hw02.add("evaluation").add("grader.py", "exercise1_utils.hpp", "exercise2_utils.hpp", "exercise1_tests", "exercise2_tests", "exercise3_tests")
    hw02.add("exercise1").add("src").add("exercise1.cc")
    hw02.add("exercise2").add("src").add("exercise2.cc")
    hw02.add("exercise3").add("src").add("exercise3.cc")
    required_dir_structure = hw02.compile()

    print("The required directory structure (Optional files are not shown):")
    print_tree(required_dir_structure)
    print()
    return required_dir_structure


def check_structure(required_dir_structure):
    def check_dir(path, required_files):
        # e.g.
        # path = "hw02"
        # required_files = [("evaluation", ["grader.py", "exercise1_utils.hpp", "exercise2_utils.hpp", "exercise1_tests", "exercise2_tests", "exercise3_tests"]),
        #                                    ("exercise1", [("src", ["exercise1.cc"])]),
        #                                    ("exercise2", [("src", ["exercise2.cc"])]),
        #                                    ("exercise3", [("src", ["exercise3.cc"])])]
        files = os.listdir(path)
        for file_structure in required_files:

            if isinstance(file_structure, str):
                # it is an ordinary file (or an empty directory)
                filename = file_structure
            else:
                # it is (non-empty) a directory...
                assert isinstance(file_structure, tuple)
                filename = file_structure[0]

            if filename not in files:
                print(f"ERROR: There must be {filename} located in {path}!")
                exit(1)

            if not isinstance(file_structure, str):
                # it is a directory...
                check_dir(path + os.path.sep + filename, file_structure[1])

    assert len(required_dir_structure) == 2  # There is one root directory (and its children).
    parent_dir_path = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
    main_dir_name = required_dir_structure[0]
    if main_dir_name not in os.listdir(parent_dir_path):
        print(f"ERROR: Your main directory must be named {main_dir_name}! "
              f"(But it is named {os.path.basename(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))}.)")
        exit(1)
    main_dir_path = parent_dir_path + os.path.sep + main_dir_name
    check_dir(main_dir_path, required_dir_structure[1])
    print("Good news: I have checked your directory structure. It looks okay!")


def perform_test(exercise_no, choosen_test):
    def build_executable():
        source_file_to_copy = os.path.dirname(
            os.path.dirname(os.path.realpath(__file__))) + os.path.sep + f"exercise{exercise_no}" + os.path.sep + "src" + os.path.sep + f"exercise{exercise_no}.cc"
        destination = os.path.dirname(os.path.realpath(__file__))
        shutil.copy2(source_file_to_copy, destination)
        argument = destination + os.path.sep + f"exercise{exercise_no}.cc"
        print("I will execute this command:")
        print(f"g++ {argument}")
        print()
        try:
            subprocess.run(["g++", argument], stdout=subprocess.PIPE, check=True).stdout.decode("utf-8")
            print()
        except:
            print()
            print("ERROR: Your program cannot be compiled!")
            exit(1)
        print("No error.")
        print()

    def calculate_grade(found_lines, expected_lines, line_grades):
        # longest common subsequence
        assert len(found_lines) == len(expected_lines) == len(line_grades)

        num = len(found_lines)
        table = [[None] * (num + 1) for _ in range(num + 1)]

        for i in range(num + 1):
            for j in range(num + 1):
                found_line = found_lines[i - 1]
                expected_line = expected_lines[j - 1]
                # We ignore the whitespace differences:
                found_line = re.sub(r"\s+", " ", found_line)
                expected_line = re.sub(r"\s+", " ", expected_line)
                if i == 0 or j == 0:
                    table[i][j] = 0
                elif found_line == expected_line:
                    table[i][j] = table[i - 1][j - 1] + line_grades[j - 1]
                else:
                    table[i][j] = max(table[i - 1][j], table[i][j - 1])

        return table[num][num]

    build_executable()
    test_path = os.path.dirname(os.path.realpath(__file__)) + os.path.sep + f"exercise{exercise_no}_tests" + os.path.sep + choosen_test

    output_file_path = test_path + os.path.sep + "output.txt"
    if not os.path.exists(output_file_path):
        print(f"ERROR: {output_file_path} must exist!")
        exit(1)

    grading_file_path = test_path + os.path.sep + "grading.txt"
    if not os.path.exists(grading_file_path):
        print(f"ERROR: {grading_file_path} must exist!")
        exit(1)

    with open(output_file_path) as f:
        expected_lines = f.read().splitlines()

    with open(grading_file_path) as f:
        line_grades = f.read().splitlines()

    if len(expected_lines) != len(line_grades):
        print(f"ERROR: {output_file_path} and {grading_file_path} must have the same number of lines!")
        exit(1)

    try:
        max_possible_grade = sum([int(grade) for grade in line_grades])
    except:
        print(f"ERROR: {grading_file_path} must contain integers only!")
        exit(1)
    if max_possible_grade != 100:
        print(f"ERROR: Sum of the grades must be 100! (But it is {max_possible_grade} for {grading_file_path}.)")
        exit(1)

    expected_lines = [line.rstrip() for line in expected_lines]
    line_grades = [int(grade) for grade in line_grades]

    if exercise_no == "1":
        argument_file_path = test_path + os.path.sep + "k.txt"
        with open(argument_file_path) as f:
            lines = f.read().splitlines()
            if len(lines) != 1:
                print(f"ERROR: There must be one line in {argument_file_path}!")
                exit(1)
            try:
                k = int(lines[0])
            except:
                print(f"ERROR: There must be an integer in {argument_file_path}!")
                exit(1)
        arguments = [test_path + os.path.sep + "scores.csv", str(k)]

    elif exercise_no == "2":
        argument_file_path = test_path + os.path.sep + "s_min_and_s_max.txt"
        with open(argument_file_path) as f:
            lines = f.read().splitlines()
            if len(lines) != 2:
                print(f"ERROR: There must be two lines in {argument_file_path}!")
                exit(1)
            try:
                s_min, s_max = int(lines[0]), int(lines[1])
            except:
                print(f"ERROR: There must be two integers in {argument_file_path}!")
                exit(1)
        arguments = [test_path + os.path.sep + "scores.csv", str(s_min), str(s_max)]

    else:
        assert exercise_no == "3"
        argument_file_path = test_path + os.path.sep + "minute.txt"
        with open(argument_file_path) as f:
            lines = f.read().splitlines()
            if len(lines) != 1:
                print(f"ERROR: There must be one line in {argument_file_path}!")
                exit(1)
            try:
                minute = int(lines[0])
            except:
                print(f"ERROR: There must be an integer in {argument_file_path}!")
                exit(1)
        arguments = [test_path + os.path.sep + "events.csv", str(minute)]

    executable_name = "a.out"
    executable_path = os.getcwd() + os.path.sep + executable_name
    if not os.path.exists(executable_path):
        print("It looks like a.out was not created. You probably use Windows :(")
        executable_name = "a.exe"
        executable_path = os.getcwd() + os.path.sep + executable_name
        if os.path.exists(executable_path):
            print("Okay, no problem! Your system created a.exe instead.")
        else:
            print("Neither a.out nor a.exe was created. There is a problem with the compiler :(")
            print("If you this error, please report this to me.")
            exit(1)

    print(f"I will execute this command ({executable_name} is the executable that was created):")
    print(f"{executable_path} {' '.join(arguments)}")
    try:
        found_output = subprocess.run([executable_path, *arguments], stdout=subprocess.PIPE, check=True).stdout.decode("utf-8")
        print()
        print("No error.")
    except:
        print()
        print("ERROR: Your program cannot be executed beyond a point! (Runtime error)")
        found_output = subprocess.run([executable_path, *arguments], stdout=subprocess.PIPE).stdout.decode("utf-8")
        print("Still, the output produced before the error will be used for evaluation.")
    print()

    found_lines = found_output.splitlines()
    found_lines = [line.rstrip() for line in found_lines]

    if len(found_lines) < len(expected_lines):
        print(f"In the output, {len(expected_lines)} lines are expected but only {len(found_lines)} lines were found.")
        print(f"{len(expected_lines) - len(found_lines)} empty lines will be added to output of your program.")
        found_lines.extend([""] * (len(expected_lines) - len(found_lines)))
    elif len(found_lines) > len(expected_lines):
        print(f"In the output, {len(expected_lines)} lines are expected but {len(found_lines)} lines were found.")
        print(f"Last {len(found_lines) - len(expected_lines)} lines will be ignored.")
        found_lines = found_lines[:len(expected_lines)]
    else:
        print(f"In the output, {len(expected_lines)} lines are expected and {len(found_lines)} lines were found. This is good.")

    assert len(found_lines) == len(expected_lines)

    grade_calculated = calculate_grade(found_lines, expected_lines, line_grades)

    if grade_calculated < 100:
        longest_expected = max([len(line) for line in expected_lines])
        longest_expected = max(longest_expected, len("Expected"))

        longest_found = max([len(line) for line in found_lines])
        longest_found = max(longest_found, len("Found"))

        print()
        print(" Points │ Success │ Expected" + " " * (longest_expected - len("Expected") + 1) + "│ Found")
        print("─" * (len("Points │ Success │ ") + longest_expected + longest_found + 4))
        for point, expected, found in zip(line_grades, expected_lines, found_lines):
            print(" " + str(point) + " " * (len("Points") - len(str(point)) + 1) + "│ ", end="")
            if re.sub(r"\s+", " ", expected) == re.sub(r"\s+", " ", found):  # We ignore the whitespace differences.
                print("Yes     │ ", end="")
            else:
                print("NO!     │ ", end="")
            print(expected + " " * (longest_expected - len(expected) + 1) + "│ ", end="")
            print(found)

    print()
    print("Grade:", grade_calculated, "/ 100")
    print("(This grade is found by computing the \"longest common subsequence\" between expected lines and found lines.)")
    print()

def main():
    show_warning()
    check_warning()
    print("\n" + "─" * 60 + "\n")
    required_dir_structure = show_structure()
    check_structure(required_dir_structure)
    print("\n" + "─" * 60 + "\n")

    while True:
        print("Choose an exercise to evaluate. Options: {1, 2, 3}")
        exercise_no = input("Choice: ")
        if exercise_no not in ("1", "2", "3"):
            print("You must choose a valid exercise number.\n")
            continue

        test_path = os.path.dirname(os.path.realpath(__file__)) + os.path.sep + f"exercise{exercise_no}_tests"
        tests = sorted(os.listdir(test_path))
        if len(tests) == 0:
            print("There is no test in the directory:", test_path)
            print()
        else:
            print()
            print("These are the available tests: ")
            print("(You can create more tests if you wish.)")
            for no, test in enumerate(tests, start=1):
                print(f"({no})", test)
            while True:
                test_no_str = input("Choice: ")
                try:
                    test_no = int(test_no_str)
                except:
                    if test_no_str in tests:
                        test_no = tests.index(test_no_str)
                        break
                    else:
                        print("You must enter a number!")
                        print()
                        continue
                if 1 <= test_no <= len(tests):
                    break
                print(f"You must enter an integer from 1 to {len(tests)}!")
                print()

            choosen_test = tests[test_no - 1]
            print(f"You have chosen {choosen_test} for exercise{exercise_no}.")
            print()
            perform_test(exercise_no, choosen_test)
            print()
            if input("Another test? ").lower() in ("no", "n", "0"):
                break
            print("\n" + "─" * 60 + "\n")


if __name__ == "__main__":
    main()


# TODO: Windows için chcp 65001 komutu gerekebilir ağaçtaki karakterleri görmek için.
# Ama yine de dosyalardan okunanlar bozuk gözüküyor.

# TODO: output'lardaki tab karakterleri problemli. Hiç kullanılmasa iyi olacak. Gerektiği kadar boşlukla değiştirilmeliler her zaman.

# TODO: Satır satır göstermeyi de LCS'e göre yapalım (diff gibi).
