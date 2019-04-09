# Description
A method combining predictive pattern mining (itemset, sequence) using a linear model with L1 regularization and statistical hypothesis test considering selection bias.  
Note that this code is created based on SafePatternPruning(https://github.com/takeuchi-lab/SafePatternPruning).
## Reference paper

- Nakagawa, K., Suzumura, S., Karasuyama, M., Tsuda, K., & Takeuchi, I. (2016, August). Safe pattern pruning: An efficient approach for predictive pattern mining. In Proceedings of the 22nd acm sigkdd international conference on knowledge discovery and data mining (pp. 1785-1794). ACM.[[pdf](http://delivery.acm.org/10.1145/2940000/2939844/p1785-nakagawa.pdf?ip=103.5.140.144&id=2939844&acc=OA&key=4D4702B0C3E38B35%2E4D4702B0C3E38B35%2E4D4702B0C3E38B35%2E4DA1889FEB74ABC0&__acm__=1554450998_1ccf9af3bd98ccd63952a7d66644afcc)]
- Suzumura, S., Nakagawa, K., Umezu, Y., Tsuda, K., & Takeuchi, I. (2017, August). Selective inference for sparse high-order interaction models. In Proceedings of the 34th International Conference on Machine Learning-Volume 70 (pp. 3338-3347). JMLR. org.[[pdf](http://proceedings.mlr.press/v70/suzumura17a/suzumura17a.pdf)]

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

### Itemset

>-0.0340430505 (19_39_75)  
>-0.0462595143 (9_10_30_36_38_53)  
>-0.0532870357 (2_3_17_22_23_38_51_73)  
>-0.0756281423 (2_85)  
>-0.0922903300 (1_11_31_32_36_58_59_62_65_70_77_95_99)  
>-0.1064798239 (4_15_26_28_55_69_76_91_93_99)  
>-0.1144663632 (10_40_44_67_78_80_87_96)  
>-0.1179265562 (20_25_35_37_43_64_68_82_83_87_89_91_99)  
>-0.1425129899 (29_38_54_57_66_75_83_98)  
>-0.1432412355 (17)  
>-0.1665651004 (11_20_59_72_95)  
>-0.1734397456 (8_27_37_42_44_49_55_58_76_80_83_91_94)  
                    ︙

For example, the line  
>-1.15 (1_2_3)

indicates the objective variable is "-1.15" and the itemset "(1,2,3)".


### Sequence

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
                ︙

For example, the line  
>-1.15 1 2 1 2 3  

indicates the objective variable is "-1.15" and the sequence "1 2 1 2 3".

## Output

For the output result, the total support, the lambda when that the pattern was added, the weight and p-value including that pattern are output in CSV for each extracted pattern as follows.  

### Itemset

>pattern,supportSum,addλ,w,p-value  
>(0),25,53,-0.251998,0.16729  
>(0_23),4,90,-0.533815,0.0327464  
>(0_31_65),2,64,0.568254,0.256419  
>(0_42),4,47,1.44459,0.376632  
>(0_49),2,43,-0.696259,0.316504  
            ︙

### Sequence

>pattern,supportSum,addλ,w,p-value  
>1 1 2 1 2 1 2 1 2 1,50,21,0.245609,0.167073  
>1 2 1 2 1 2 1 2 1 2,233,1,0.586735,1.09544e-26  
>1 2 1 2 2,128,28,-0.0590423,4.185e-05  
>2 1 2 1 2 1 2 1 2 1,233,1,0.607691,9.99603e-28  
            ︙

# License
GNU General Public License v3.0
