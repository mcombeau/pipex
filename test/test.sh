#!/bin/bash

#Test files
INPUT=test/envp.txt
INPUT_INVALID=test/none.what
INPUT_INFINITE=/dev/urandom
OUTPUT_EXPECTED=test/expected_output.txt
OUTPUT_PIPEX=test/output.txt
OUTPUT_INVALID=test/forbidden.txt

#Test shortcuts
COMMAND_WITH_PATH=/usr/bin/cat
LEAK_TOGGLE=0           #Set to 1 to run Valgrind, some other value for no check
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all"

#Colors
NC="\033[0m"
CYAN="\033[36m"
BOLD="\033[1m"
ULINE="\033[4m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
PURPLE="\033[35m"

cd ..
printf "${YELLOW}${BOLD}\n=========== MAKEFILE =============\n${NC}${PURPLE}"
make re
#printf "${YELLOW}${BOLD}\n========== NORMINETTE ============\n${NC}${CYAN}"
#norminette

printf "${PURPLE}${BOLD}\n==================================\n"
printf                   "|           MANDATORY            |"
printf                  "\n==================================\n${NC}"

printf "${YELLOW}${BOLD}\n============ TEST 1 ==============\n${NC}"
printf "Basic function test.\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT cat | wc -l > $OUTPUT_EXPECTED\n${NC}${YELLOW}"
<$INPUT cat | wc -l > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"cat\" \"wc -l\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT "cat" "wc -l" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "cat" "wc -l" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n============ TEST 2 ==============\n${NC}"
printf "Grepping for a string that exists in the input file.\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT cat | grep USER= > $OUTPUT_EXPECTED${NC}${YELLOW}\n"
<$INPUT cat | grep USER= > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"cat\" \"grep USER=\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT "cat" "grep USER=" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "cat" "grep USER=" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n============ TEST 3 ==============\n${NC}"
printf "Grepping for a string that ${BOLD}does not${NC} exist in the input file.\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT grep abracadabra | wc -l > $OUTPUT_EXPECTED${NC}${YELLOW}\n"
<$INPUT grep abracadabra | wc -l > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"grep abracadabra\" \"wc -l\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT "grep abracadabra" "wc -l" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "grep abracadabra" "wc -l" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n============ TEST 4 ==============\n${NC}"
printf "Command already contains the path. If this does not work, check command path with 'which cat' and modify it in variables above\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT $COMMAND_WITH_PATH | wc -l > $OUTPUT_EXPECTED${NC}${YELLOW}\n"
<$INPUT $COMMAND_WITH_PATH | wc -l > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT $COMMAND_WITH_PATH \"wc -l\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT $COMMAND_WITH_PATH "wc -l" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT $COMMAND_WITH_PATH "wc -l" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n============ TEST 5 ==============\n${NC}"
rm -f $OUTPUT_EXPECTED $OUTPUT_PIPEX
touch $OUTPUT_EXPECTED $OUTPUT_PIPEX
printf "Input file does not exist. Error message should contain \"No such file or directory\"\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT_INVALID cat | wc -l > $OUTPUT_EXPECTED${NC}${YELLOW}\n"
< $INPUT_INVALID cat | wc -l > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT_INVALID \"cat\" \"wc -l\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT_INVALID "cat" "wc -l" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT_INVALID "cat" "wc -l" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n============ TEST 6 ==============\n${NC}"
printf "Command 1 does not exist. Error message should contain \"command not found\"\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT fakecommand | wc -l > $OUTPUT_EXPECTED${NC}${YELLOW}\n"
<$INPUT fakecommand | wc -l > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"fakecommand\" \"wc -l\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT "fakecommand" "wc -l" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "fakecommand" "wc -l" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n============ TEST 7 ==============\n${NC}"
printf "Command 2 does not exist. Error message should contain \"command not found\"\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT cat | fakecommand > $OUTPUT_EXPECTED${NC}${YELLOW}\n"
<$INPUT cat | fakecommand > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"cat\" \"fakecommand\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT "cat" "fakecommand" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "cat" "fakecommand" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n============ TEST 8 ==============\n${NC}"
printf "No commands exist. Error message should contain \"command not found\"\nand there should be no weird overlapping display.\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT fakecommand | fakecommand > $OUTPUT_EXPECTED${NC}${YELLOW}\n"
<$INPUT fakecommand | fakecommand > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"fakecommand\" \"fakecommand\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT "fakecommand" "fakecommand" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "fakecommand" "fakecommand" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n============ TEST 9 ==============\n${NC}"
rm -f $OUTPUT_INVALID
touch $OUTPUT_INVALID
chmod 000 $OUTPUT_INVALID
printf "Output file can't be opened. Error message should contain \"Permission denied\"\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT cat | wc -l > $OUTPUT_INVALID${NC}${YELLOW}\n"
<$INPUT cat | wc -l > $OUTPUT_INVALID
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"cat\" \"wc -l\" $OUTPUT_INVALID${NC}\n${YELLOW}"
./pipex $INPUT "cat" "wc -l" $OUTPUT_INVALID
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "cat" "wc -l" $OUTPUT_INVALID
fi

printf "${YELLOW}${BOLD}\n============ TEST 10 ==============\n${NC}"
printf "Input file is infinite. Should output one line with random chars. Should not hang.\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT_INFINITE cat | head -1 > $OUTPUT_EXPECTED${NC}${YELLOW}\n"
<$INPUT_INFINITE cat | head -1 > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT_INFINITE \"cat\" \"head -1\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT_INFINITE "cat" "head -1" $OUTPUT_PIPEX
printf "${NC}Expected output like:\n"
cat $OUTPUT_EXPECTED
printf "\nGot pipex output:\n"
cat $OUTPUT_PIPEX
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT_INFINITE "cat" "head -1" $OUTPUT_PIPEX
fi

printf "${PURPLE}${BOLD}\n==================================\n"
printf                   "|            BONUS               |"
printf                  "\n==================================\n${NC}"
printf "${YELLOW}${BOLD}\n========= BONUS TEST 1 ===========\n${NC}"
printf "Handling 3 commands test.\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT cat | grep PATH | wc -c > $OUTPUT_EXPECTED\n${NC}${YELLOW}"
<$INPUT cat | grep PATH | wc -c > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"cat\" \"grep PATH\" \"wc -c\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT "cat" "grep PATH" "wc -c" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "cat" "grep PATH" "wc -l" $OUTPUT_PIPEX
fi

printf "${YELLOW}${BOLD}\n========= BONUS TEST 2 ===========\n${NC}"
printf "Handling 4 commands test.\n"
printf "Shell command: ${BOLD}${BLUE}<$INPUT cat | grep PATH | grep usr/ | wc -c > $OUTPUT_EXPECTED\n${NC}${YELLOW}"
<$INPUT cat | grep PATH | grep usr/ | wc -c > $OUTPUT_EXPECTED
printf "${NC}Pipex command: ${BOLD}${BLUE}./pipex $INPUT \"cat\" \"grep PATH\" \"grep usr/\" \"wc -c\" $OUTPUT_PIPEX${NC}\n${YELLOW}"
./pipex $INPUT "cat" "grep PATH" "grep usr/" "wc -c" $OUTPUT_PIPEX
printf "${NC}Output file: "
if cmp -s $OUTPUT_EXPECTED $OUTPUT_PIPEX; then
    printf "${GREEN}${BOLD}OK!${NC}\n"
else
    printf "${RED}${BOLD}KO: Output differs${NC}:\n"
    diff --color -c $OUTPUT_EXPECTED $OUTPUT_PIPEX
fi
if [ $LEAK_TOGGLE -eq 1 ]; then
    printf "Leak check:${CYAN}\n"
    $VALGRIND ./pipex $INPUT "cat" "grep PATH" "wc -l" $OUTPUT_PIPEX
fi