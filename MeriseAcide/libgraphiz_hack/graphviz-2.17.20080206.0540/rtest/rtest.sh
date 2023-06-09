#! /bin/ksh
#
# Graphviz regression test driver
#
# Also relies on strps.awk.
#
# TODO:
#  Fix to allow multiple test data sets depending
# on architecture, etc. Maintain common ones in REFDIR.
#  Report differences with shared version and with new output.

TESTFILE=tests.txt     # Test specifications
GRAPHDIR=graphs        # Directory of input graphs and data
OUTDIR=ndata           # Directory for test output
REFDIR=nshare          # Directory for expected test output
GENERATE=              # If set, generate test data
VERBOSE=               # If set, give verbose output
NOOP=                  # If set, just print list of tests
DOT=../cmd/dot/dot_builtins # build tree version with a builtin set of plugins
		       # should be $(top_builddir)/cmd/dot/dot_static
DIFFIMG=../contrib/diffimg/diffimg # build tree version

TESTNAME=   # name of test
GRAPH=      # graph specification
IDX=
typeset -i i j SUBTESTCNT
typeset -i LINECNT=0
typeset -A TESTTYPES
typeset ALG[10]
typeset FMT[10]
typeset FLAGS[10]
TMPINFILE=tmp$$.dot
TMPFILE1=tmpnew$$
TMPFILE2=tmpref$$

# Read single line, storing it in LINE and update count.
# Return 0 on success.
function readLine
{
  if read -u 3 LINE
  then
    (( LINECNT+=1 ))
    return 0
  else
    return 1
  fi
}

# Skip blank lines and comments (lines starting with #)
# Use first real line as the test name
function skipLines
{
  while readLine
  do
    if [[ -n $LINE && ${LINE:0:1} != \# ]]
    then
      return 0
    fi
  done
  return 1
}

# Subtests have the form: layout format optional_flags
# Store the 3 parts in the arrays ALG, FMT, FLAGS.
# Stop at a blank line
function readSubtests
{
  (( SUBTESTCNT=0 ))
  while readLine
  do
    if [[ -z "$LINE" ]]
    then
      return
    fi
    if [[ ${LINE:0:1} != \# ]]
    then
      echo $LINE | read ALG0 FMT0 FLAGS0
      ALG[$SUBTESTCNT]=$ALG0
      FMT[$SUBTESTCNT]=$FMT0
      FLAGS[$SUBTESTCNT]=$FLAGS0
      (( SUBTESTCNT+=1 ))
    fi
  done
}

function readTest
{
  # read test name
  if skipLines
  then
    TESTNAME=$LINE
  else
    return 1
  fi

  # read input graph
  if skipLines
  then
    GRAPH=$LINE
  else
    return 1
  fi

  readSubtests
  return 0
}

# newfile = $1
# oldfile = $2
# assume subscript indicates file type
function strip {
  case $1 in
    *.ps )
      awk -f strps.awk $1 > $TFILE1
      awk -f strps.awk $2 > $TFILE2
      ;;
    *.svg )
      sed '/^<!--/d' < $1 | sed '/-->$/d' > $TFILE1
      sed '/^<!--/d' < $2 | sed '/-->$/d' > $TFILE2
      ;;
    * )
      cp $1  $TFILE1
      cp $2  $TFILE2
      ;;
  esac

}

# Compare old and new output and report if different.
#  Args: testname index fmt
function doDiff
{
   FILE1=$OUTDIR/$OUTFILE
   FILE2=$REFDIR/$OUTFILE
   F=${3%%:*}
   case $F in
    ps | ps2 )
      awk -f strps.awk $FILE1 > $TMPFILE1
      awk -f strps.awk $FILE2 > $TMPFILE2
      diff -q $TMPFILE2 $TMPFILE1 > /dev/null 
      ;;
    svg )
      sed '/^<!--/d' < $FILE1 | sed '/-->$/d' > $TMPFILE1
      sed '/^<!--/d' < $FILE2 | sed '/-->$/d' > $TMPFILE2
      diff -q $TMPFILE2 $TMPFILE1 > /dev/null 
      ;;
    png )
      $DIFFIMG $FILE2 $FILE1 > /dev/null 
      ;;
    * )
      diff -q $FILE2 $FILE1 > /dev/null 
      ;;
    esac
    if [[ $? != 0 ]]
    then
	    print -u 2 "Test $1:$2 : == Failed == $OUTFILE"
    fi
}

# Generate output file name given 3 parameters.
#   testname layout format
# If format ends in :*, remove this, change the colons to underscores,
# and append to basename
# If the last two parameters have been used before, add numeric suffix.
function genOutname
{
  if [[ $3 == *:* ]]
  then
    F=${3%%:*}
    XFMT=${3#$F}
    XFMT=${XFMT/:/_}
  else
    F=$3
    XFMT=""
  fi

  IDX="$2$XFMT$F"
  j=${TESTTYPES[$IDX]}
  if (( j == 0 ))
  then
    TESTTYPES[$IDX]=1
    J=""
  else
    TESTTYPES[$IDX]=$(( j+1 ))
    J=$j
  fi 
  OUTFILE="$1_$2$XFMT$J.$F"
}

# clear out all entries of associated array
function aunset #name
{
	typeset i
	nameref v=$1
	for i in ${!v[@]}
	do	unset v[$i]
	done
}

function doTest
{
  if (( SUBTESTCNT == 0 ))
  then
    return
  fi
  case $GRAPH in
    = )
      INFILE=$GRAPHDIR/$TESTNAME.dot
      ;;
    graph* | digraph* )
      INFILE=$TMPINFILE
      echo "$GRAPH" > $INFILE
      ;;
    *.dot )
      INFILE=$GRAPHDIR/$GRAPH
      ;;
    * )
      echo "Unknown graph spec, test $TESTNAME - ignoring"
      return
      ;;
  esac

  for ((i=0;i<SUBTESTCNT;i++))
  do
    genOutname $TESTNAME ${ALG[$i]} ${FMT[$i]}
    OUTPATH=$OUTDIR/$OUTFILE
    KFLAGS=${ALG[$i]}
    TFLAGS=${FMT[$i]}
    test -z "$KFLAGS" || KFLAGS="-K$KFLAGS"
    test -z "$TFLAGS" || TFLAGS="-T$TFLAGS"
    testcmd="$DOT $KFLAGS $TFLAGS ${FLAGS[$i]} -o$OUTPATH $INFILE"
    if [[ -n "$VERBOSE" ]]
    then
      print $testcmd
    fi
    if [[ $NOOP == 1 ]]
    then
      continue
    fi
    
    $testcmd 2> errout
    RVAL=$?

    if [[ $RVAL != 0 || ! -s $OUTPATH ]]
    then
      print -u 2 "Test $TESTNAME:$i : == Layout failed =="
    elif [[ $GENERATE == 1 ]]
    then
      continue
    elif [[ -r $REFDIR/$OUTFILE ]]
    then
      doDiff $TESTNAME $i ${FMT[$i]}
    else
      print -u 2 "Test $TESTNAME:$i : == No file $REFDIR/$OUTFILE for comparison =="
    fi
  done

  # clear TESTTYPES
  aunset TESTTYPES
#  for W in ${!TESTTYPES[@]}
#  do
#    TESTTYPES[$W]=0
#  done 
}

trap 'rm -f $TMPFILE1 $TMPFILE2 $TMPINFILE errout; exit' 0 1 2 3 15

Usage='rtest [-gvn]\n
 -g : generate test data\n
 -v : verbose\n
 -n : print test'

while getopts :gnv c
do
  case $c in
  n )
    VERBOSE=1
    NOOP=1
    ;;
  v )
    VERBOSE=1
    ;;
  g )
    GENERATE=1
    if [[ ! -d "$REFDIR" ]]
    then
		mkdir $REFDIR
    fi
    OUTDIR=$REFDIR
    ;;
  :)
    echo $OPTARG requires a value
    exit 2
    ;;
  \? )
    if [[ "$OPTARG" == '?' ]]
    then
      echo $Usage
      exit 0
    else
      echo "rtest: unknown flag $OPTARG - ignored"
    fi
    ;;
  esac
done
shift $((OPTIND-1))

# Check environment and initialize

if [[ ! -d "$REFDIR" ]]
then
  print -u 2 "Test data directory $REFDIR does not exit"
  exit 1
fi

if [[ ! -d "$OUTDIR" ]]
then
  mkdir $OUTDIR
fi

if [[ ! -x $DOT ]]
then
  print -u 2 "$DOT program is not executable"
  exit 1
fi

if [[ ! -x $DIFFIMG ]]
then
  print -u 2 "$DIFFIMG program is not executable"
  exit 1
fi


exec 3< $TESTFILE
while readTest
do
  doTest
done
