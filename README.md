# Description
A method combining prediction sequence mining using a linear model with L1 regularization and statistical hypothesis test considering selection bias.  
Note that this code is created based on SafePatternPruning(https://github.com/takeuchi-lab/SafePatternPruning).

# Verified Environmental
- gcc version 6.4.0
- GNU Make 3.81
- Eigen 3.3.4(Install yourself)

# Files
```
.
├── README.md
└── src
    ├── Eigen/
    ├── crossvalidation.cc
    ├── crossvalidation.h
    ├── database.cc
    ├── database.h
    ├── lassospp.cc
    ├── lassospp.h
    ├── learnerspp.h
    ├── makefile
    ├── modelselector.h
    ├── prefixspan.cc
    ├── prefixspan.h
    ├── selectiveInference.cc
    ├── selectiveInference.h
    └── train.cc
```

# Setup
```
$ cd src/ && make
```

# Usage
```
$ ./train [-options] [filename]
```

## Option
```
    -t : learning regularization parameter index (besides CV) (default:min lambda)
    -m : min support (default 1)
    -L : max pattern length  (default 10)
    -T : number of regularization path (default 100)
    -r : min regularization path ratio (default 2.0)
    -i : max iteration (default 1000000)
    -f : frequency of convergence check (default 50)
    -e : tolerance (default 1e-6)
    -F : output filename (default output/result.csv)
    -p : max event interval (default 0|-1:none)
    -c : is cross validation (default 0:false|1:true)
        -k : k-fold (default:10)
        -a : average (default:10)
    -P : is cross validation for performance evaluation (default 0:false|1:true)
    -I : importance sampling(default 1:true|0:false)
    -R : sampling(importance sampling , default 1e+8)
    -C : is CloSpan (default 0:false|1:true)
    -s : is support frequency (default 0:false|1:true)
```

## Example
```
./train -L 30 -F output/result.csv data/test.txt
```

# Demo

## Input Data

As shown in the following example, input data should have a obvective variable(real number) at the beginning and a sequence of symbols behind it. Note that separate each column with space, and the symbol must a positive integer.  
>1.1532180855 1 2 1 2 2 1 2 1 2 1 2 3 3 4 1 3 4 3 4 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 2 3 4 4 1   
>0.4647166093 1 2 1 2 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2  
>-0.1800180228 1 2 1 2 3 1 2 1 2 1 2 4 3 4 1 1 2 1 2 1 2 1 2 1 2 1 2 3 3 4 3 4 3 4 3 4 3 4 3 4 3  
>-0.6434264106 1 2 1 2 4 1 2 1 2 1 2 1 3 4 3 4 4 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1  
>-0.1432412355 3 4 3 4 4 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 2 3 4 3 4 3 4 3 4 3 4 3 4 4  
>3.1031693637 1 2 1 2 1 2 3 3 4 3 3 4 3 4 3 1 2 14 3 4 3 4 3 4 3 4 4  
>-0.3882494977 1 2 1 2 3 1 2 1 2 1 2 3 1 2 1 2 1 2 1 2 1 2 1 2 3 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 2 3 4 3 4 3 4 3 4 3 4 3 4 3 4 4  
>1.5801588357 1 2 1 2 2 3 4 3 3 4 3 4 4 1 2 1 2 1 2 1 2 1 2 1 2 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 3 4 3 4 3 4 3 4 3 4 3  
>0.9166487651 1 2 1 2 1 2 2 3 4 2 1 2 1 2 1 2 1 2 1 2 1 2 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 3 3 4 3 4 3 4 3 4 4  
>-1.1884857828 1 2 1 2 4 3 4 2 3 4 3 4 2 1 2 1 2 1 2 1 2 1 2 1 2 1 3 4 3 4 3 4 3 4 3 4 3 4 3  

For example, the line  
>-1.15 1 2 1 2 3  

indicates the objective variable is "-1.15" and the sequence "1 2 1 2 3".

## Output

For the output result, the total support, the lambda when that the pattern was added, the weight and p-value including that pattern are output in CSV for each extracted pattern as follows.  

>pattern,supportSum,addλ,w,p-value  
>1 1 2 1 2 1 2 1 2 1,50,21,0.245609,0.167073  
>1 2 1 2 1 2 1 2 1 2,233,1,0.586735,1.09544e-26  
>1 2 1 2 2,128,28,-0.0590423,4.185e-05  
>2 1 2 1 2 1 2 1 2 1,233,1,0.607691,9.99603e-28  
>2 1 2 1 2 1 2 1 2 4,89,25,0.148739,0.00307875  
>1 2 1 2 1 2 1 2 4 1,28,29,1.97289e-05,0.115662  
>1 2 1 2 4 3 4,93,24,-0.213491,6.2259e-08  
>2 1 2 1 2 1 2 4 1 2,28,27,0.0598328,0.115524  

# License
GNU General Public License v3.0