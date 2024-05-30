#!/bin/bash
FAIL_COUNT=0
RUN_DIR=temp
MINISHELL=../minishell
mkdir -p $RUN_DIR

function test {
    echo "Test: $1"
    echo "$1" | bash > $RUN_DIR/bash.out 2> $RUN_DIR/bash.err
    exit_bash=$?
    echo "$1" | $MINISHELL > $RUN_DIR/minishell.out 2> $RUN_DIR/minishell.err
    exit_minishell=$?
    cat $RUN_DIR/minishell.out
    cat $RUN_DIR/minishell.err
    DIFF=$(diff $RUN_DIR/bash.out $RUN_DIR/minishell.out)
    if [ "$DIFF" != "" ]; then
        echo "DIFF stdout:"
        echo $DIFF
        FAIL_COUNT=$((FAIL_COUNT+1))
    fi
    DIFF=$(diff $RUN_DIR/bash.err $RUN_DIR/minishell.err)
    if [ "$DIFF" != "" ]; then
        # error message may differ
        if [ $(cat $RUN_DIR/bash.err | wc -l) != $(cat $RUN_DIR/minishell.err | wc -l) ]; then
            echo "DIFF stderr:"
            echo $DIFF
            FAIL_COUNT=$((FAIL_COUNT+1))
        fi
    fi
    if [ $exit_bash != $exit_minishell ]; then
        echo "DIFF exit code:"
        echo "bash: $exit_bash"
        echo "minishell: $exit_minishell"
        FAIL_COUNT=$((FAIL_COUNT+1))
    fi
    echo $'----------------------------'
}

test $'nocmd'
test $'ls -l | grep d| wc -l'
test $'cat << EOF\nhello\nEOF'
test $'cat << EOF\n$HOME\nhello\nEOF'
test $'cat 1\t2   3'
test $'no_cmd\necho $?aaa'
test $'echo $"HOME"$USER'
test $'echo "$ x"'
test $'echo $>temp/out_41 | cat -e temp/out_41'

#expander
test $'export a="echo hi"\n$a "<d"|cat -e'
test $'export a="aa\'bb"\necho $a'
test $'export var=\'aa $? bb\'\necho $var'
test $'export a=\' 1\'\nexport b=\'2 \'\nexport c=\'3 3\'\necho $a$b$c'
test $'export a="  "\necho $a 12$a"34"'
test $'export a="  "\n$a echo hi'

test $'/ls'
test $'./ls -la'
test $'example_bin'
test $'./example_bin'
test $'>temp/out_920 | ls -l temp/out_920\nrm temp/out_920'

#builtins
test $'echo 1\nexit 42\necho 2'
test $'echo 1\nexit \necho 2'
test $'echo 1\nexit aa \necho 2'
test $'echo 1\nexit 1 2 \necho 2'
test $'ls | exit 42 \n echo $?'
test $'exit 42 | ls no-file \n echo $?'
test $'cd .. \n pwd'
test $'cd no_dir'
test $'cd files ..'
test $'cd .. | cat \n pwd'
test $'pwd arg'
test $'echo'
test $'echo -nnn -nn nn -nn hello -n | cat -e'
#test $'mkdir a\n cd a\n mkdir b \n cd b \n rm -r ../../a \n pwd \n cd .. \n cd $HOME \n pwd'
test $'export var \n export |grep var \n env| grep var '
test $'export var=123 \n export var \n echo $var'
test $'export var=123 | cat \n echo var: $var' 
test $'export 1var=abc'

exit $FAIL_COUNT