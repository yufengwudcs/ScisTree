# convert monovar called SNVs to haploid probabilities
# usage: awk -f xxx.awk c=xxx output.vcf
# output.vcf: monovar called variants 
# c: number of cells
# output: use // to seperate reads from sites;; each line: <n0> <n1> from a cell/site 

BEGIN{ ns = 0;
}

{
  if( substr($1,1,1) != "#" )
  {
#printf "NF: %d, line: %s\n", NF, $0;
    if( NF != 10+c)
    {
      printf "Cell number: wrong\n";
      exit(1);
    }
    for(i=10; i<=NF-1; ++i)
    {
      listGenosStep[i-9] = getGenotype($i);
    }
    if( isGoodSNV(listGenosStep) == 1 )
    {
      ++ns;
      for(i=10; i<=NF-1; ++i)
      {
        getReadCountsAt( $i, listReadCounts, ns, i-9 );   
      }
    }
    else
    {
#      printf "This is not a good SNV: %s\n", $0;
    }
  }
}

END{
  # estimate prior
  for(i=1; i<=ns; ++i)
  {
    printf "//\n";
    #priorp = estimatePriorProb(i);
#printf "Site %d: priorp: %f\n", i, priorp;
    for(j=1; j<=c; ++j)
    {
      printf "%d %d\n", listReadCounts[i, j, 1], listReadCounts[i,j,2];
    }
  }

}

function getReadCountsAt(fieldVCF, listReadCountsIn, ns1, index2)
{
  g = substr(fieldVCF, 1, 3);
  if( g != "0/0" && g != "0/1" && g != "1/1" )
  {
#printf "Empty\n";
    # this is empty 
    listReadCountsIn[ns1, index2, 1] = 0;
    listReadCountsIn[ns1, index2, 2] = 0;
  }
  else
  {
#printf "field: %s\n", fieldVCF;
    # start from back
    split(fieldVCF, listFields, ":");;
    # find phred scores in the last fields
    split(listFields[2], listSubfields, ",");
    listReadCountsIn[ns1, index2, 1] = listSubfields[1] + 0;
    listReadCountsIn[ns1, index2, 2] = listSubfields[2] + 0;
  }

}



function getGenotype(geno01)
{
  # take the 0/0, 0/1/ and 1/1
  g = substr(geno01, 1, 3);
  if( g == "0/0")
  {
    return "0";
  }
  else if( g == "0/1" || g == "1/1")
  {
    return "1";
  }
  else
  {
    return "-";
  }
}

function estimatePriorProb(sitecurr1 )
{
  # is this a good SNV: must have at least two 1
  resEPP = 0.5;
  numZeros = 0;
  numOnes = 0;
  for(ii1=1; ii1<=c; ++ii1)
  {
    if(listGenosProbIn[sitecurr1, ii1] >= 0.0  )
    { 
      if( listGenosIn[sitecurr1,ii1] == "1")
      {
        ++numOnes;
      }
      else if( listGenosIn[sitecurr1,ii1] == "0"  )
      {
        ++numZeros;
      }
    }
  }
  if( numOnes+numZeros >= 2 )
  {
    resEPP = (1.0*numZeros)/(numZeros+numOnes);
    if( resEPP < 0.000001)
    {
      resEPP = 0.000001;
    }
  }
  return resEPP;
}



function isGoodSNV(listGenotypeStep )
{
  # is this a good SNV: must have at least two 1
  numOnes = 0;
  for(ii2=1; ii2<=c; ++ii2)
  {
    if( listGenotypeStep[ii2] == "1")
    {
      ++numOnes;
    }
  }
  if( numOnes >= 2 )
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

function getPhredScoreForZero(vcfGeno)
{
  # if nothing is reported
  if(vcfGeno == "./.")
  {
    return -1.0;
  }

  # start from back
  split(vcfGeno, listFields, ":");;
  # find phred scores in the last fields
  split(listFields[5], listSubfields, ",");

  if(listSubfields[1] == "0" && listSubfields[2] == "0" && listSubfields[3] == "0")
  {
    return -1.0;
  }

  p0 = calcBase10Exp( listSubfields[1] );
printf "p0: %f, input: %d\n", p0, listSubfields[1]; 
  p1 = calcBase10Exp( listSubfields[2] );
printf "p1: %f, input: %d\n", p1, listSubfields[2]; 
  p2 = calcBase10Exp( listSubfields[3] );
printf "p2: %f, input: %d\n", p2, listSubfields[3]; 
  psum = p0+p1+p2;
  if( psum <= 0.0 )
  {
    return 0.5;
  }
  res =  p0/psum;
  if( res < 0.00001)
  {
    res = 0.000001;
  }
#printf "prob: %f, for input %s\n", res, vcfGeno;
  return res;
}

function calcBase10Exp( val )
{
  if( val >= 100.0)
  {
    return 0.0;
  }
  return exp((-1.0*val)/10*log(10.0));
}


