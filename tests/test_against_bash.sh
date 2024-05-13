#!/bin/bash
FAIL_COUNT=0
RUN_DIR=temp
MINISHELL=../minishell/minishell
mkdir -p $RUN_DIR

function test {
    echo "Test: $1"
    echo "$1" | bash > $RUN_DIR/bash.out 2> $RUN_DIR/bash.err
    exit_bash=$?
    echo "$1" | $MINISHELL > $RUN_DIR/minishell.out 2> $RUN_DIR/minishell.err
    exit_minishell=$?
    DIFF=$(diff $RUN_DIR/bash.out $RUN_DIR/minishell.out)
    if [ "$DIFF" != "" ]; then
        echo "stdout diff:"
        echo $DIFF
        FAIL_COUNT=$((FAIL_COUNT+1))
    fi
    DIFF=$(diff $RUN_DIR/bash.err $RUN_DIR/minishell.err)
    if [ "$DIFF" != "" ]; then
        # error message may differ
        if [ $(cat $RUN_DIR/bash.err | wc -l) != $(cat $RUN_DIR/minishell.err | wc -l) ]; then
            echo "stderr diff:"
            echo $DIFF
            FAIL_COUNT=$((FAIL_COUNT+1))
        fi
    fi
    if [ $exit_bash != $exit_minishell ]; then
        echo "exit code diff:"
        echo "bash: $exit_bash"
        echo "minishell: $exit_minishell"
        FAIL_COUNT=$((FAIL_COUNT+1))
    fi
}

test $'nocmd'
test $'cd .. \n pwd'
test $'ls -l | grep d| wc -l'
test $'cat << EOF\nhello\nEOF'
test $'cat << EOF\n$HOME\nhello\nEOF'
test $'cat 1\t2   3'
test $'no_cmd\necho $?aaa'
test $'echo $"HOME"$USER'
test $'echo "$ x"'
test $'echo $>temp/out_41 | cat -e temp/out_41'
test $'export no_val \n export |grep no_val \n env| grep no_val '
test $'export a="echo hi"\n$a "<d"|cat -e'
test $'export a="aa\'bb"\necho $a'
test $'export var=\'aa $? bb\'\necho $var'
test $'export a=\' 1\'\nexport b=\'2 \'\nexport c=\'3 3\'\necho $a$b$c'
test $'export a="  "\necho $a 12$a"34"'
test $'export a="  "\n$a echo hi'
#test $'>temp/out_920 | ls -l temp/\nrm temp/out_920'
exit $FAIL_COUNT