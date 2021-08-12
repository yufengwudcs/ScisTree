# ScisTree

ScisTree is a computer program for inferring cell tree and calling genotypes from uncertain single cell genotype data. If you use ScisTree, please cite: 

Accurate and Efficient Cell Lineage Tree Inference from Noisy Single Cell Data: the Maximum Likelihood Perfect Phylogeny Approach, Yufeng Wu, Bioinformatics, https://doi.org/10.1093/bioinformatics/btz676, 36(3):742-750, 2020.

The key feature of ScisTree is that it works with uncertain genotypes with individualized probability. That is, you can specify for each genotype (at a row/cell or column/site) different probabilities of being a particular genotype state. ScisTree allows both binary or ternary genotypes. Here is an example for binary genotypes. Note: don't include blank rows in the input genotype file.

This is an example for binary genotypes (don't leave blank lines between rows; the blank lines here are meant to improve readability).



HAPLOID 5 4 c1 c2 c3 c4 c5

s1 0.8 0.02 0.8 0.8

s2 0.02 0.02 0.02 0.8

s3 0.8 0.02 0.02 0.8

s4 0.02 0.8 0.8 0.8

s5 0.8 0.02 0.8 0.02

The first line specifies the type of genotypes (HAPLOID or TERNARY), number of sites and number of cells, along with cell names. Lines started with # are ignored by ScisTree. You should have one line for each site. For each line, it starts with the site name; then it follows by the probability of being genotype 0 (for binary genotypes) or being 0 and 1 (consecutively for ternary genotypes) for each cells. For example, in the above example, the first 0.8 on the first row means this genotype is equal to 0 with probability 0.8. Here is an example for ternary genotypes.

This is an example for ternary genotypes.


TERNARY 5 4 c1 c2 c3 c4

s1 0.8 0.05 0.02 0.1 0.8 0.05 0.8 0.01

s2 0.02 0.1 0.02 0.15 0.02 0.05 0.8 0.1

s3 0.8 0.1 0.02 0.1 0.02 0.15 0.8 0.1

s4 0.02 0.1 0.8 0.05 0.8 0.15 0.8 0.1

s5 0.8 0.1 0.02 0.1 0.8 0.15 0.02 0.1

Here, the first 0.8 and 0.05 on the first row mean the genotype of the first cell has probability 0.8 of being 0, and probability 0.05 of being 1 (and thus probability of 0.15 being genotype 2).

Once you have the executable for ScisTree, simple type to run:

./scistree example.dat

This would infer a cell tree based on maximum likelihood. If you want to impute the genotypes, type:

./scistree -v example.dat

Refer to the user manual for more details on how to use ScisTree.

# About sequence reads 
A frequently asked question is how to deal with single cell sequence reads in order to use ScisTree. For your convenience, I have created a simple example with detailed instructions. To try this, you should download and unzip the two zipped files called SeqReadsTest.tar.gz and SeqReadsTest-sortbam.tar.gz. Unzipped these files and move the sorted bam files to the directory "SeqReadsTest". Then follow the instructions given in README.txt. It should be easy to try it out. Note: you will need the program Monovar and samtools to run the example. I would suggest you to first install and try to run Monovar before running this tuotiral.

August 29, 2019: I was told the script "dumpCellReadCounts.awk" was missing from the distribution. Use "convMonovarOutputToAlleleCounts.awk" instead.

# Code release, May 28, 2019
Current version is v.1.2.0.6. The source code is in the file ScisTree-ver1.2.0.6-src.zip. There are sevreal changes since the previous release. First the input format is changed to allow user to specify cell and site names. Also, a main change in this code release is that ScisTree now allows to discard some genotypes when constructing initial trees. Simulation shows that this can be useful when data contains significant noise. In this case, using only more reliable genotypes can improve the quality of initial trees.

I have released the source code of ScisTree. To build it, simply de-compress it and then type "make" from the source code directory. That should be all you need.

# Branch length added, August 10, 2021
I added a branch length option (-l). Please note that branch length is only for reference. This option has been implemented before but I didn't advertise it. 
