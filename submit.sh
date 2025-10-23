set -e
set -o xtrace

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 task-name"
    exit 1
fi

TASK=$1
../run_linter.sh "$TASK"
git add "../$TASK"
git commit -m "solve $TASK"
git push
