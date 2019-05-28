# ScisTree

ScisTree is a computer program for inferring cell tree and calling genotypes from uncertain single cell genotype data. If you use ScisTree, please cite: 

Accurate and Efficient Cell Lineage Tree Inference from Noisy Single Cell Data: the Maximum Likelihood Perfect Phylogeny Approach, Yufeng Wu, manuscript, 2019.

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

# Code release, May 28, 2019
Current version is v.1.2.0.6. There are sevreal changes since the previous release. First the input format is changed to allow user to specify cell and site names. Also, a main change in this code release is that ScisTree now allows to discard some genotypes when constructing initial trees. Simulation shows that this can be useful when data contains significant noise. In this case, using only more reliable genotypes can improve the quality of initial trees.

I have released the source code of ScisTree soon. To build it, simply de-compress it and then type "make" from the source code directory. That should be all you need.
