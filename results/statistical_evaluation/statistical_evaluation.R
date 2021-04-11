"+" = function(x,y) {
    if(is.character(x) || is.character(y)) {
        return(paste(x , y, sep=""))
    } else {
        .Primitive("+")(x,y)
    }
}



#' ---
#' title: "Example of the statistical evaluation of experimental results"
#' author: "Christian Blum <christian.blum@iiia.csic.es>"
#' date: "Today"
#' output: pdf_document
#' ---
#' 

#' This document contains the code used for the statistical assessment of empirical results
#' of algorithm comparision
#' 
#' ## Data loading and variable initializations
#' 
#' Load the package and create directories for the results of the analysis

library(scmamp)
#library(reshape2)
#library(ggplot2)
kx <- 3
filex <- "obj-real-all.csv"
data.file <- filex
sep <- ","

plot.dir<-"./plots"
tex.dir<-"./tables"
dir.create(plot.dir , showWarnings = FALSE)
dir.create(tex.dir , showWarnings = FALSE)

#' First of all, we will load the results.
#' 
#' Now we create a summarization table with the average results per instance
#' 


data <- read.csv(data.file, sep=sep, header = FALSE) #, colClasses=c("n", "m", "k",  "type", "objKojic", "objFaria", "objNew1", "objNew2"), "NULL", "NULL")
# maximization
print(data)
tip  <- "LB"

kol3 <- "VNS"
kol1 <- "Greedy"
kol4 <- "BS-basic"
kol2 <- "Restricted-BS"
kol5 <- "BS-VNS"
kol6 <- "A*"

colnames(data) <- c( "instances",  kol1, kol2, kol3, kol4, kol5, kol6) #"m", "n", "p", "k",
 
 
#names(data)<- c("n", "m", "k", "type",   "FdSdS", "New-1", "New-2") # "ix", "m", "n", 

data[, "VNS"] = -1 * data[, "VNS"]
data[, "Greedy"] = -1 * data[, "Greedy"]
data[, "BS-basic"] = -1 * data[, "BS-basic"]
data[, "Restricted-BS"] = -1 * data[, "Restricted-BS"]
data[, "BS-VNS"] = -1 * data[, "BS-VNS"]
data[, "A*"] = -1 * data[, "A*"]

colnames(data) <- c( "instances",  kol1, "restricted-BS", kol3, "basic-BS", "BS&VNS", kol6 ) #"n", "m", "p", "k",
print(data)

average.function <- mean
instance.descriptors <- c("instances")  #"n", "m", "p", "k")
to.ignore <- ""


summary.data <- summarizeData(data=data, fun=average.function, 
                              group.by=instance.descriptors, ignore=to.ignore)

#' ## Statistical assessment
#' 
#' First, verify that there is at least one algorithm that is different from the rest.
#' For that we can use Friedman's test

#' 

alg.columns <- c( kol1, "restricted-BS", kol3, "basic-BS", "BS&VNS", kol6 )
#data1 <- data[, alg.columns]
#cols <-  c(  "FdSdS", "New-1", "New-2")
#friedmanTest(data1)

#' Now, two-fold post-hoc comparison. For the table, the best in each instance vs. the rest
#' 

all.vs.best.results <- postHocTest(data=data, algorithms=alg.columns, 
                                   group.by=instance.descriptors, test="friedman",
                                   control="max", use.rank=FALSE, sum.fun=average.function,
                                   correct="finner", alpha=0.05)

#' Now, all vs. all for the plot.
#' 

all.vs.all <- postHocTest(data=data, algorithms=alg.columns, test="friedman",
                          control=NULL, use.rank=TRUE, sum.fun=average.function,
                          correct="finner", alpha=0.05, decreasing=FALSE)

#' Create a Criticial Difference Plot
#' 

plot <- "/ranking_plot_global_obj_real.pdf" #sigma_2
pdf(file=paste0(plot.dir, plot), width=8, height=4.2)
plotRanking(all.vs.all$corrected.pval, summary=all.vs.all$summary, alpha=0.05)
dev.off()

# ## Table
#' 
#' Create a latex table. In bold, the best for each row.
#' 

#aux <- apply(summary.data[, -(1:2)], MARGIN=1, FUN=function(i) {return(i==min(i))})
#aux <- apply(summary.data[, -(1,2,6)], MARGIN=1, FUN=function(i) {return(i==max(i))})
#bold.matrix <- cbind(FALSE, FALSE, t(aux))

# We mark those with no significant differences with an *. All, except those in bold!!

#aux <- all.vs.best.results$corrected.pval>0.05
#aux <- aux & !bold.matrix
#mark.matrix <- cbind(FALSE, FALSE, aux[, -c(1:2)])
#mark.matrix <- cbind(FALSE, FALSE, aux[, -c(1,2,6)])


#table.path <- paste0(tex.dir, "/table.tex")
#writeTabular(table=summary.data, file=table.path, format="f", bold=bold.matrix, 
#             mark=mark.matrix, mark.char="\\bigstar", align="l", print.row.names=FALSE, 
#             digits=c(0,0,2,2,2,2), vrule=2, hrule=c(0, 5, 10, 15, 20, 25, 30))
#

