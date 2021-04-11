#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <vector>
#include <string>
#include<stdarg.h>
#ifdef  __linux__ 
#include<unistd.h>
#endif //  __linux__ 
#include "random.h"
#include "gclcs_vns.h"


#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

/* promenljive */
problemStruct* problem;
vnsStruct* vns;
std::vector<char> solution;
int fitMaxChecks = 0;
int fitRealChecks = 0;

void tee(char const* fmt, ...) {
	//no output when irace=1
	if (vns->iraceMode)
		return;
	FILE* f = NULL;
	f = fopen(vns->imelog, "at");
	if (f == NULL) {
		printf("Greska pri otvaranju log fajla.\n");
		exit(EXIT_FAILURE);
	}
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	va_start(ap, fmt);
	vfprintf(f, fmt, ap);
	va_end(ap);
	fclose(f);
}

int validValue(char c) {
	return c >= 'a' && c < 'a' + problem->sig;
}

double fv1(int card, int nonCorrect) {
	if (nonCorrect > 0)
		return nonCorrect;
	return nonCorrect + (problem->n * 1.0 -card) / problem->n;
}

double fv1(int card, int nonCorrect, double avg, double stdev) {
	return nonCorrect + stdev / 1000;
}

int subCount(char* super, char* sub, int superLen, int subLen) {
	int subFound = 0, i;
	for (i = 0; i < superLen && subFound<subLen; i++)
		if (sub[subFound] == super[i])
			subFound++;
	return subFound;
}

int leftMostPath(char X[MAXS], char Y[MAXS], int m, int n, int Ypath[MAXS]) {
	int i=0, j=0, k=0;
	while(i < m && j < n) {
		if (!validValue(Y[j])) {
			j++;
			continue;
		}
		else if(Y[j] == X[i]) {
			Ypath[j] = i;
			j++;
			k++;
			i++;
		}
		else
			i++;
	}
	return k;
}/* trazi najlevlje pojavljivanje Y unutar X i upisuje to u Ypath*/

int lcs(char X[MAXS], char Y[MAXS], int m, int n)
{
	static int L[MAXS][MAXS];
	for (int i = 0; i <= m; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			if (i == 0 || j == 0)
				L[i][j] = 0;
			else if (X[i - 1] == Y[j - 1])
				L[i][j] = L[i - 1][j - 1] + 1;
			else
				L[i][j] = max(L[i - 1][j], L[i][j - 1]);
		}
	}
	return L[m][n];
}

int compactSolutionArray(char* x, char *cx) {
	int i, len = 0;
	for (i = 0; i < problem->n; i++)
		if (validValue(x[i]))
			cx[len++] = x[i];
	cx[len] = '\0';
	return len;
}


int lcsWithPath(char X[MAXS], char Y[MAXS], int m, int n, int Xpath[MAXS], int Ypath[MAXS])
{
	int i, j;
	static int L[MAXS][MAXS];
	for (i = 0; i <= m; i++)
	{
		for (j = 0; j <= n; j++)
		{
			if (i == 0 || j == 0)
				L[i][j] = 0;
			else if (X[i - 1] == Y[j - 1])
				L[i][j] = L[i - 1][j - 1] + 1;
			else
				L[i][j] = max(L[i - 1][j], L[i][j - 1]);
		}
	}

	i = m, j = n;
	while (i > 0 && j > 0)
	{
		// If current character in X[] and Y are same, then 
		// current character is part of LCS 
		if (X[i - 1] == Y[j - 1])
		{
			Xpath[i - 1] = j - 1; // Put current character in result 
			Ypath[j - 1] = i - 1;
			i--; j--;     // reduce values of i, j and index 
		}

		// If not same, then find the larger of two and 
		// go in the direction of larger value 
		else if (L[i - 1][j] > L[i][j - 1])
			i--;
		else
			j--;
	}

	return L[m][n];
}


void calculateStats(int Xpath[MAXP][MAXS], int card, double* avgCount, double* stdev) {
	int counts[MAXS], j, i;
	*avgCount = 0;
	*stdev = 0;
	int nonZeroCount = 0;
	for (j = 0; j < card; j++)
		counts[j] = 0;

	for (i = 0; i < problem->np; i++) {
		for (j = 0; j < card; j++)
			if (Xpath[i][j] != -1)
				counts[j]++;
	}
	for (j = 0; j < card; j++) {
		if (counts[j] == 0)
			continue;
		*avgCount += counts[j];
		nonZeroCount++;
	}
	if (nonZeroCount > 0)
		*avgCount /= nonZeroCount;
	for (j = 0; j < card; j++) {
		if (counts[j] == 0)
			continue;
		*stdev += (counts[j] - *avgCount) * (counts[j] - *avgCount);
	}
	if (nonZeroCount > 0)
		*stdev = sqrt(*stdev / nonZeroCount);
}

void printSolutionDetails(solStruct* s) {
	char solClean[MAXS];
	int Xpath[MAXP][MAXS];
	int Ypath[MAXP][MAXS];
	double avgCount, stdev;

	int j;
	int card, nonCorrect, i, sc, sc2;
	card = compactSolutionArray(s->x, solClean);
	nonCorrect = 0;

	for (i = 0; i < problem->np; i++) {
		for (j = 0; j < card; j++)
			Xpath[i][j] = -1;
		for (j = 0; j < problem->nnp[i]; j++)
			Ypath[i][j] = -1;
		sc = lcsWithPath(solClean, problem->p[i], card, problem->nnp[i], Xpath[i], Ypath[i]);
		nonCorrect += (problem->nnp[i] - sc);
		for (j = 0; j < card; j++)
			if (Xpath[i][j] == -1)
				tee(" ");
			else
				tee("%c", problem->p[i][Xpath[i][j]]);
		tee("%s\n", solClean);
		tee("\n%s\t%d\n", problem->p[i], sc);
		for (j = 0; j < problem->nnp[i]; j++)
			if (Ypath[i][j] == -1)
				tee(" ");
			else
				tee("%c", solClean[Ypath[i][j]]);
		putchar('\n');
	}
	calculateStats(Xpath, card, &avgCount, &stdev);

	tee("%s\t%d\tavg=%.2lf\tstdev=%.2lf\n", solClean, nonCorrect, avgCount, stdev);

}

int nonCorrectInSuperStringsAfterInsert(solStruct* s, int i, char c) {
	return 0;
}/* ispituje da li se isplati ubaciti karakter c ispred aktuelne pozicije i */

int calculateLCSPaths(char superString[MAXS], char subString[MAXS], int m, int n, int rightMostYpath[MAXS], int leftMostYpath[MAXS]) {
	char tmpSearch[MAXS];
	char leftMatch[MAXS];
	char rightMatch[MAXS];
	int tmpXpath[MAXS];
	int leftMostCS;
	int j,k;
	int sc;

	for (j = 0; j < n; j++) {
		rightMostYpath[j] = -1;
		leftMostYpath[j] = -1;
	}
	sc = lcsWithPath(superString,subString, m, n, tmpXpath, rightMostYpath);
	for (j = 0; j < n; j++)
		if (rightMostYpath[j] == -1)
			tmpSearch[j] = 'a' + problem->sig;
		else
			tmpSearch[j] = subString[j];
	leftMostCS = leftMostPath(superString, tmpSearch, m, n, leftMostYpath);

	if (sc != leftMostCS)
		leftMostCS = leftMostPath(superString, tmpSearch,m, n, leftMostYpath);
	assert(sc == leftMostCS);

	k = 0;
	for (j = 0; j < n; j++)
		if (rightMostYpath[j] != -1)
			rightMatch[k++] = superString[rightMostYpath[j]];
	rightMatch[k] = '\0';
	k = 0;
	for (j = 0; j < n; j++)
		if (leftMostYpath[j] != -1)
			leftMatch[k++] = superString[leftMostYpath[j]];
	leftMatch[k] = '\0';
	//tee("%s\n%s\n%s\n", solClean, rightMatch,leftMatch);
	assert(strcmp(rightMatch, leftMatch) == 0);
	return sc;
}

double fitness(solStruct* s, int comparisonNonCorrect, int applyCompact) {
	int i, j,k,sc, reverseSc;
	int maxNoncorrect = 0;
	solCachedValueStruct cachedFv;
	double fv;
	char solClean[MAXS];
	s->card = 0;
	s->nonCorrect = 0;
	fitMaxChecks += (problem->ns + problem->np);
	s->card = compactSolutionArray(s->x, solClean);
	if (applyCompact) {
		//primenjujemo ovu kompaktifikaciju
		for (i = 0; i < s->card; i++)
			s->x[i] = solClean[i];
		for (; i < problem->n; i++)
			s->x[i] = 'a' + problem->sig;
	}

	//prolazimo kroz sve sablone i belezimo nepoklapanja rezultujuceg stringa
	for (i = 0; i < problem->np; i++) {
		fitRealChecks++;
		for (j = 0; j < s->card; j++) {
			s->rightMostYpath[i+problem->ns][j] = -1;
			s->leftMostYpath[i+problem->ns][j] = -1;
		}
		sc = calculateLCSPaths(problem->p[i],solClean, problem->nnp[i], s->card, s->rightMostYpath[i+problem->ns], s->leftMostYpath[i+problem->ns]);
		s->sc[i+problem->ns] = sc;

		//sc = lcs(solClean, problem->p[i], s->card, problem->nnp[i]);
		s->nonCorrect += (problem->nnp[i] - sc);
		if (comparisonNonCorrect != -1 && s->nonCorrect > comparisonNonCorrect) {//izlazimo ranije, ovo je za parcijalna racunanja kada nam je samo bitno da vidimo da li smo poboljsali
			fv = fv1(s->card, s->nonCorrect);
			return fv;
		}
	}

	//prolazimo kroz sve nadstringove i belezimo nepoklapanja rezultujuceg stringa
	for (i = 0; i < problem->ns; i++) {
		fitRealChecks++;
		for (j = 0; j < s->card; j++) {
			s->rightMostYpath[i][j] = -1;
			s->leftMostYpath[i][j] = -1;
		}
		sc = calculateLCSPaths(problem->s[i], solClean, problem->nns[i], s->card, s->rightMostYpath[i], s->leftMostYpath[i]);
		s->sc[i] = sc;
	
		s->nonCorrect += (s->card - sc);
		if (comparisonNonCorrect != -1 && s->nonCorrect > comparisonNonCorrect) {//izlazimo ranije, ovo je za parcijalna racunanja kada nam je samo bitno da vidimo da li smo poboljsali
			fv = fv1(s->card, s->nonCorrect);
			return fv;
		}
	}

	fv = fv1(s->card, s->nonCorrect);
	return fv;
}

int partialLSC(solStruct* s, int card, int isSuperstring, int i, int pos, int c, int isInsert) {
	int nextPos, sc, found, end, prevPos, start, j, ylen;
	int *rightMostYpath, *leftMostYpath;
	char* y;
	if (isSuperstring) {
		y = problem->s[i];
		ylen = problem->nns[i];
		leftMostYpath = s->leftMostYpath[i];
		rightMostYpath = s->rightMostYpath[i];
		sc = s->sc[i];
	}
	else {
		y = problem->p[i];
		ylen = problem->nnp[i];
		leftMostYpath = s->leftMostYpath[i+problem->ns];
		rightMostYpath = s->rightMostYpath[i+problem->ns];
		sc = s->sc[i + problem->ns];
	}
	if (isInsert) {
		//proveravamo samo region od najlvevljeg uparivanja za poziciju pre poz i najdesnijeg uparivanja za poziciju posle pos da li sadrzi karater c
		//posto se karakter c umece na poziciju pos, ona prethodna pozicija pos ce postati prva posle nje, a pos-1 ako je >=0 ce postati prva pre nje
		nextPos = pos;
		found = 0;
		while (nextPos < card) {
			if (rightMostYpath[nextPos] != -1) {
				found = 1;
				break;
			}
			nextPos++;
		}
		if (found)
			end = rightMostYpath[nextPos] - 1;
		else
			end = ylen - 1;//ako nismo nasli, onda je region proveravanja do kraja nadsekvence

		prevPos = pos - 1;
		found = 0;
		while (prevPos >= 0) {
			if (leftMostYpath[prevPos] != -1) {
				found = 1;
				break;
			}
			prevPos--;
		}
		if (found)
			start = leftMostYpath[prevPos] + 1;
		else
			start = 0;
		for (j = start; j <= end; j++)
			if (y[j] == c) {
				//ako region sadrzi trazeni karakter to ce uticati na povecanje LCS sigurno
				//takodje, ako region ne sadrzi karakter, moguce je da ce se takodje povecati LCS, ali takve izmene ne hvatamo ovim brzim fitnessom
				sc++;
				break;
			}
	}
	else {
		//proveravamo region izmedju najlevljeg uparivanja pozicije pre i najdesnijeg uparivanja pozicije posle, aktivna pozicija ce biti ponistena pa ako je bilo uparena, mora se oduzeti od sc

		//for (j = 0; j < card; j++)
		//	if (s->rightMostYpath[i][j] != -1)
		//		tee("%c", problem->s[i][s->rightMostYpath[i][j]]);
		//putchar('\n');
		//for (j = 0; j < card; j++)
		//	if (s->leftMostYpath[i][j] != -1)
		//		tee("%c", problem->s[i][s->leftMostYpath [i] [j] ]);
		//putchar('\n');

		if (rightMostYpath[pos] != -1 || leftMostYpath[pos] != -1)
			sc--;
		nextPos = pos + 1;
		found = 0;
		while (nextPos < card) {
			if (rightMostYpath[nextPos] != -1) {
				found = 1;
				break;
			}
			nextPos++;
		}
		if (found)
			end = rightMostYpath[nextPos] - 1;
		else
			end = ylen - 1;//ako nismo nasli, onda je region proveravanja do kraja nadsekvence

		prevPos = pos - 1;
		found = 0;
		while (prevPos >= 0) {
			if (leftMostYpath[prevPos] != -1) {
				found = 1;
				break;
			}
			prevPos--;
		}
		if (found)
			start = leftMostYpath[prevPos] + 1;
		else
			start = 0;
		for (j = start; j <= end; j++)
			if (y[j] == c) {
				//ako region u nadnisci sadrzi trazeni karakter to ce uticati na povecanje LCS sigurno
				//takodje, ako region ne sadrzi karakter, moguce je da ce se takodje povecati LCS, ali takve izmene ne hvatamo ovim brzim fitnessom
				sc++;
				break;
			}
	}
	return sc;
}

double fastFitness(solStruct* s, int comparisonNonCorrect, int pos, char c, int isInsert) {
	int i, j, k, sc,scCheck;
	int maxNoncorrect = 0;
	int found;
	solCachedValueStruct cachedFv;
	double fv;
	int start, end, nextPos, prevPos;
	char solClean[MAXS], solCleanNew[MAXS];
	int card = 0, cardNew = 0;
	int nonCorrect = 0;
	fitMaxChecks += (problem->ns + problem->np);
	//card = compactSolutionArray(s->x, solClean);
	card = s->card;
	cardNew = card;
	if(isInsert)
		cardNew = card + 1;

	/*
	for (i = 0; i < pos; i++)
		solCleanNew[i] = solClean[i];
	solCleanNew[pos] = c;
	while (i < card) {
		solCleanNew[i + 1] = solClean[i];
		i++;
	}*/

	//prolazimo kroz sve sablone i belezimo nepoklapanja rezultujuceg stringa
	for (i = 0; i < problem->np; i++) {
		//scCheck = lcs(solCleanNew, problem->p[i], cardNew, problem->nnp[i]);
		sc = partialLSC(s, card, 0, i, pos, c, isInsert);
		//if (sc != scCheck) {
		//	sc = partialLSC(s, card, 0, i, pos, c, isInsert);
		//}
		nonCorrect += (problem->nnp[i] - sc);
		if (comparisonNonCorrect != -1 && nonCorrect > comparisonNonCorrect) {//izlazimo ranije, ovo je za parcijalna racunanja kada nam je samo bitno da vidimo da li smo poboljsali
			fv = fv1(cardNew, nonCorrect);
			return fv;
		}
	}

	//prolazimo kroz sve nadstringove i belezimo nepoklapanja rezultujuceg stringa
	for (i = 0; i < problem->ns; i++) {
		sc = partialLSC(s, card, 1, i, pos, c, isInsert);
		nonCorrect += (cardNew - sc);
		if (comparisonNonCorrect != -1 && nonCorrect > comparisonNonCorrect) {//izlazimo ranije, ovo je za parcijalna racunanja kada nam je samo bitno da vidimo da li smo poboljsali
			fv = fv1(cardNew, nonCorrect);
			return fv;
		}
	}

	fv = fv1(cardNew, nonCorrect);
	return fv;
}


void encode(char s[MAXS], int len) {
	int i, j, k, kf;
	for (j = 0; j < len; j++) {
		if (s[j] == '\n')
			break;
		for(k=0; problem->fromChar[k]!='\0'; k++)
			if (problem->fromChar[k] == s[j]) {
				s[j] = problem->toChar[k];
				break;
			}
		kf = -1;
		for (k = 0; k < problem->inputAlphabetCount; k++)
			if (problem->inputAlphabet[k] == s[j]) {
				kf = k;
				break;
			}
		if (kf == -1) {
			problem->inputAlphabet[problem->inputAlphabetCount++] = s[j];
			kf = problem->inputAlphabetCount - 1;
		}
		s[j] = 'a' + kf;
	}
	s[len] = '\0';
}

void randInit() {
	int i;
	for (i = 0; i < problem->n; i++)
		if (GetDRandom(0, 1) < 0.1)
			problem->x.x[i] = 'a' + GetRandom(problem->sig);
		else
			problem->x.x[i] = 'a' + problem->sig;
}

void nullInit() {
	int i;
	for (i = 0; i < problem->n; i++)
		problem->x.x[i] = 'a' + problem->sig;
}

void fileInit(FILE *fp) {
	char c;
	int i = 0;
	int len;
	c = fgetc(fp);
	while(c!=EOF){
		problem->x.x[i++] = c;
		c = fgetc(fp);
	}
	len = i;
	while (i < problem->n)
		problem->x.x[i++] = '_';
	problem->x.x[i] = '\0';
	tee("Ucitano originalno inicijalno resenje duzine %d %s\n",len, problem->x.x);
	encode(problem->x.x, len);
	tee("Ucitano enkodirano inicijalno resenje %s\n", problem->x.x);
}

/* generisanje pocetnog resenja */
void init()
{
	int i,j;
	char newc,oldc;
	double fastFv, controlFv;
	int okCnt=0, notOkCnt=0;
	FILE* fInit = NULL;

	vns->kmax = vns->kmaxinit;
	if (vns->kmax > problem->n)
		vns->kmax = problem->n;
	vns->k = vns->kmin;
	vns->iter = 0;
	vns->iterp = 0;
	vns->nls = 0;
	vns->nlsp = 0;
	vns->initialFileSolution = 0;

	fInit = fopen(vns->imeInit, "r");
	if (fInit == NULL) {
		tee("Nije dostavljen ulazni fajl sa pocetnim resenjem pa se sprovodi standardna inicijalizacija.\n");
		//randInit();
		nullInit();
		problem->x.fv = fitness(&problem->x, -1,1);
		LS1(&problem->x,1);
	}
	else {
		tee("Dostavljen je fajl %s pa se sprovodi inicijalizacija sa datim pocetnim resenjem.\n", vns->imeInit);
		fileInit(fInit);
		fclose(fInit);
		problem->x.fv = fitness(&problem->x, -1,1);
		if (problem->x.nonCorrect != 0) {
			tee("Ulazno inicijalno resenje nije dopustivo jer ima %d nezadovoljenih uslova, prekidamo izvrsavanje!\n", problem->x.nonCorrect);
			if (vns->iraceMode)
				printf("val: 0\ntime: 0\n");
			exit(EXIT_FAILURE);
		}
		vns->initialFileSolution = problem->x.card;
	}

	problem->x.fv = fitness(&problem->x,-1,1);
	copyt();
} /* init */

  /* alokacija memorije */

void allocation()
{
	int i;
	problem->x.x = (char*)malloc(problem->n * sizeof(char));
	problem->xt.x = (char*)malloc(problem->n * sizeof(char));
	vns->v = (int*)malloc(problem->n * sizeof(int));
	vns->y = (int*)malloc(problem->n * sizeof(int));

	problem->nns = (int*)malloc(problem->ns * sizeof(int));
	problem->s = (char**)malloc(problem->ns * sizeof(char*));
	for (i = 0; i < problem->ns; i++) {
		problem->s[i] = (char*)malloc(MAXS * sizeof(char));
	}
	problem->x.sc = (int*)malloc((problem->ns + problem->np) * sizeof(int));
	problem->x.leftMostYpath = (int**)malloc((problem->ns + problem->np) * sizeof(int*));
	problem->x.rightMostYpath = (int**)malloc((problem->ns + problem->np) * sizeof(int*));
	problem->xt.sc = (int*)malloc((problem->ns + problem->np) * sizeof(int));
	problem->xt.leftMostYpath = (int**)malloc((problem->ns + problem->np) * sizeof(int*));
	problem->xt.rightMostYpath = (int**)malloc((problem->ns + problem->np) * sizeof(int*));
	for (i = 0; i < problem->ns + problem->np; i++) {
		problem->x.leftMostYpath[i] = (int*)malloc(MAXS * sizeof(int));
		problem->x.rightMostYpath[i] = (int*)malloc(MAXS * sizeof(int));
		problem->xt.leftMostYpath[i] = (int*)malloc(MAXS * sizeof(int));
		problem->xt.rightMostYpath[i] = (int*)malloc(MAXS * sizeof(int));
	}

	problem->nnp = (int*)malloc(problem->np * sizeof(int));
	problem->p = (char**)malloc(problem->np * sizeof(char*));

	for (i = 0; i < problem->np; i++) {
		problem->p[i] = (char*)malloc(MAXS * sizeof(char));
	}

	if (problem->x.x == NULL || problem->xt.x == NULL || vns->v == NULL || problem->nns == NULL 
		|| problem->s == NULL  || problem->nnp == NULL || problem->p == NULL || problem->x.sc==NULL || problem->xt.sc==NULL 
		|| problem->x.leftMostYpath==NULL || problem->x.rightMostYpath==NULL || problem->xt.leftMostYpath == NULL || problem->xt.rightMostYpath == NULL)
	{
		tee("Nesto ne moze da se alocira!!!!\n");
		exit(0);
	}
} /* allocation */

void deallocation()
{
	int i;
	free(problem->x.x);
	free(problem->xt.x);
	for (i = 0; i < problem->ns; i++) {
		free(problem->s[i]);
	}
	for (i = 0; i < problem->np; i++)
		free(problem->p[i]);
	free(problem->s);
	free(problem->p);
	free(problem->nns);
	free(problem->nnp);
	free(problem);
	free(vns->y);
	free(vns->v);
	free(vns);
} /* allocation */

  /* prelazimo u novo resenje */
void copy(solStruct* dest, solStruct* src)
{
	int i;
	dest->fv = src->fv;
	dest->card = src->card;
	dest->nonCorrect = src->nonCorrect;
	for (i = 0; i < problem->n; i++) {
		dest->x[i] = src->x[i];
	}
	for (i = 0; i < problem->np + problem->ns; i++) {
		dest->leftMostYpath[i] = src->leftMostYpath[i];
		dest->rightMostYpath[i] = src->rightMostYpath[i];
		dest->sc[i] = src->sc[i];
	}
} /* kopiraj */

void copyt()
{
	time(&vns->tp);
	vns->runp = clock();
	vns->iterp = vns->iter;
	vns->nlsp = vns->nls;
} /* kopvreme */

void checkFeasibilityPotential() {
	int i, j, sc, ok, okCnt = 0;
	for (i = 0; i < problem->ns; i++) {
		ok = 1;
		for (j = 0; j < problem->np; j++) {
			sc = subCount(problem->s[i], problem->p[j], problem->nns[i], problem->nnp[j]);
			if (sc != problem->nnp[j]) {
				ok = 0;
				break;
			}
		}
		if (!ok)
			tee("Nadniska pod rednim brojem %d nije u skladu sa podniskom %d\n", i + 1, j + 1);
		else
			okCnt++;
	}
	tee("%d od ukupno %d nadniski je u skladu sa podniskama.\n", okCnt, problem->ns);
	if (okCnt != problem->ns) {
		tee("Resenje ne moze nikako da bude dopustivo!\n");
		if (vns->iraceMode) 
			printf("val: 0\ntime: 0\n");
	}

}/* proverava da li su sve podniske unutar nadniski, u suprotnom nema sanse da se pronadje dopustivo resenje */

void input(void)
{
	int i, j, tmp, tmp2;

#ifdef  __linux__ 
	char cwd[MAXS];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		tee("Trenutni radni direktorijum je: %s\n", cwd);
	}
	else {
		perror("getcwd() error");
		return 1;
	}
#endif

	vns->inp = fopen(vns->imeul, "rt");
	FILE* fpCh = fopen("characters.change", "r");

	if (vns->inp == NULL)
	{
		tee("Ulazna datoteka %s ne moze da se otvori\n", vns->imeul);
		exit(0);
	}
	problem->fromChar[0] = '\0';
	problem->toChar[0] = '\0';
	if (fpCh != NULL) {
		fscanf(fpCh, "%s", problem->fromChar);
		fscanf(fpCh, "%s", problem->toChar);
		tee("Postoji datoteka za zamenu karaktera pa je sprovodimo i menjamo %s u %s\n", problem->fromChar, problem->toChar);

	}
	problem->n = MAXS;
	fscanf(vns->inp, "|S|=%d Sigma=%d |P|=%d k=%d", &problem->ns, &tmp2, &tmp, &problem->np);
	allocation();

	problem->inputAlphabetCount = 0;
	for (i = 0; i < problem->ns; i++) {
		fscanf(vns->inp, "%d %s", &problem->nns[i], problem->s[i]);
		if (problem->nns[i] < problem->n)
			problem->n = problem->nns[i];
		problem->s[i][problem->nns[i]] = '\0';
		encode(problem->s[i], problem->nns[i]);
		//tee("Nandiska %d = %s\n", i, problem->s[i]);
	}
	tee("Dimenzija ulaznog problema je jednaka najvise najmanjoj nadsekvenci i iznosi %d\n", problem->n);

	for (i = 0; i < problem->np; i++) {
		fscanf(vns->inp, "%d %s", &problem->nnp[i], problem->p[i]);
		problem->p[i][problem->nnp[i]] = '\0';
		encode(problem->p[i], problem->nnp[i]);
		//tee("Sablon %d = %s\n", i, problem->p[i]);
	}
	problem->inputAlphabet[problem->inputAlphabetCount] = '\0';
	problem->sig = problem->inputAlphabetCount;
	checkFeasibilityPotential();
	if(fpCh!=NULL)
		fclose(fpCh);
	fclose(vns->inp);
} /* input */

bool subseq(int i)  // ckeck if solution is a substring of s_i
{

	int sub_len = 0;
	//tee("sol[0]--------------------------------------------------_> %d = %d \n", solution[0] - 'a', problem->s[ i ][ 0 ] - 'a' ); 
	for (int ix = 0; ix < problem->nns[i] && sub_len < solution.size(); ++ix)
		if (solution[sub_len] - 'a' == problem->s[i][ix] - 'a')
			sub_len++;
	if (sub_len == solution.size())
		return true;
	else
		return false;
}

bool superseq(int i) // P_i subsequence of solution? 
{
	int matching = 0; //tee("sol[0]--------------------------------------------------_> %d", solution[0]); 
	for (int ix = 0; ix < solution.size() && matching < problem->nnp[i]; ++ix)
		if (problem->p[i][matching] - 'a' == solution[ix] - 'a')
			matching++;
	if (matching == problem->nnp[i])
		return true;
	else
		return false;
}

//problem->s; problem->p[i]
bool validate()
{
	int i = 0;
	for (i = 0; i < problem->n; i++)
		if (validValue(problem->x.x[i]) && problem->x.x[i] != ' ')
		{
			char c = problem->x.x[i];
			solution.push_back(c);
		}
	while (i < problem->ns)
	{
		bool s = subseq(i);
		if (!s) {
			// tee(" rjesenje nije sadrzano u s_%d ", i);
			return false;
		}
		i++;
	}
	// check if patterns are included into solution
	i = 0;
	while (i < problem->np)
	{
		bool s = superseq(i);
		if (!s) {
			//tee("Patern nije sadrzan u rjesenju, indeks: %d ", i); 
			return false;
		}
		i++;
	}
	return true;
}

void print(FILE* f)
{
	int i, solSize, index;
	fprintf(f, "\n\n\n");
	fprintf(f, "Komandna linija: %s\n", vns->cmdl);
	fprintf(f, "Instanca: %s\nIzvrsavanje je pocelo: %s Zavrseno %s Trajalo je: %16.4lf sekundi   Broj iteracija=%d  LS iteracija=%d\n"
		, vns->imeul, vns->sts, vns->ste, vns->rt, vns->iter, vns->nls);
	fprintf(f, "Konacno resenje je dobijeno u %s posle %16.4lf sekundi  Broj iteracija=%d  LS iteracija=%d\n"
		, vns->stp, vns->rtp, vns->iterp, vns->nlsp);
	fprintf(f, "Funkcija cilja je:   %lf\n", problem->x.fv);
	fprintf(f, "Dopustivost: %d\n", problem->x.fv < 1);
	solSize = 0;
	for (i = 0; i < problem->n; i++)
		if (validValue(problem->x.x[i]) && problem->x.x[i] != ' ')
		{
			solSize++;
			fprintf(f, "%c", DEC(problem->x.x[i]));
		}
	fprintf(f, "\n");
	fprintf(f, "Kardinalnost resenja je %d \n", solSize);
	if (vns->initialFileSolution != 0)
		fprintf(f, "Inicijalno resenje iz fajla je bilo kardinalnosti: %d\n", vns->initialFileSolution);
	else
		fprintf(f, "Nije korisceno inicijalno resenje iz fajla.\n");
	fprintf(f, "Ulazni alfabet duzine %d je %s\n",problem->sig, problem->inputAlphabet);

} /* stampaj */

void stopTime() {
	time(&vns->tend);
	vns->runend = clock();
	vns->rt = ((double)(vns->runend - vns->runstart)) / CLOCKS_PER_SEC;
	vns->rtp = ((double)(vns->runp - vns->runstart)) / CLOCKS_PER_SEC;
	strcpy(vns->sts, ctime(&vns->tstart));
	strcpy(vns->ste, ctime(&vns->tend));
	strcpy(vns->stp, ctime(&vns->tp));
}

  /* Stampanje izlaznih rezultata */
void printAll()
{
		int i, k;
		FILE* fr=NULL, *fs=NULL;
		int max, maxk;
		char allSolName[MAXS];
		FILE* flog = NULL;
		flog = fopen(vns->imelog, "at");
		if (flog == NULL) {
			printf("Greska pri otvaranju log fajla.\n");
			exit(EXIT_FAILURE);
		}

		tee("Validating...");
		bool valid = validate();
		if (valid)
			tee("Rjesenje je validno");
		else
			tee("Rjesenje nije validno");

		sprintf(allSolName, "results_%d_%d_%.2f_%d_%d_%d.sol", vns->kmin, vns->kmax, vns->prob, vns->iterMax, vns->iterWoChange, vns->maxTime);

		fr = fopen(allSolName, "at");

		if (fr == NULL)
		{
			tee("Izlazna datoteka %s ne moze da se otvori\n", allSolName);
			exit(0);
		}

		fs = fopen(vns->imeout, "at");
		if (fs == NULL)
		{
			tee("Izlazna datoteka %s ne moze da se otvori\n", vns->imeout);
			exit(0);
		}

		print(stdout);
		print(flog);
		print(fr);
		print(fs);
		fclose(fr);
		fclose(fs);
		fclose(flog);

	} /* kompletan ispis u razlicitim fajlovima */

void shakingChange(solStruct* sol) {
	int i;
	char newc;
	/* menjamo k elemenata */
	for (i = 0; i < vns->k; i++)
		vns->v[i] = GetRandom(problem->n - i);

	variation(problem->n, vns->k, vns->v, vns->v, vns->y);
	for (i = 0; i < vns->k; i++) {
		newc = 'a' + GetRandom(problem->sig);
		//tee("Menjam karakter %c na poziciji %d sa karakterom %c\n", sol->x[vns->v[i]], vns->v[i], newc);
		sol->x[vns->v[i]] = newc;
	}

}/* klasicni shaking sa nasumicnim izmenama */

void shakingDelete(solStruct* sol) {
	int i, k;
	char newc;
	/* brisemo k elemenata */
	for (i = 0; i < problem->n; i++)
		vns->v[i] = GetRandom(problem->n - i);

	variation(problem->n, problem->n, vns->v, vns->v, vns->y);
	i = 0;
	k = 0;
	while (i<problem->n && k<vns->k) {
		if (!validValue(sol->x[vns->v[i]])) {
			i++;
			continue;
		}
		sol->x[vns->v[i]] = 'a' + problem->sig;
		k++;
		i++;
	}

}/* klasicni shaking sa nasumicnim izmenama */

void shakingAll(solStruct* sol) {
	int i, k;
	char newc;
	for (i = 0; i < problem->n; i++)
		vns->v[i] = GetRandom(problem->n - i);

	variation(problem->n, problem->n, vns->v, vns->v, vns->y);
	i = 0;
	k = 0;
	while (i < problem->n && k < vns->k) {
		do {
			newc = newc = 'a' + GetRandom(problem->sig + 1);
		} while (newc == sol->x[vns->v[i]]);
		tee("%d. %c -> %c\n",vns->v[i], sol->x[vns->v[i]], newc);
		sol->x[vns->v[i]] = newc;
		k++;
		i++;
	}

}/* klasicni shaking sa nasumicnim izmenama */

void shakingTargeted(solStruct* sol) {
	int i = 0, ri;
	while (i < vns->k) {
		ri = GetRandom(sol->card);
		sol->x[ri] = 'a' + GetRandom(problem->sig);
		i++;
	}
	/* k elemenata na kraju sekvence dobijaju nasumicne vrednosti */
	//for (i = sol->card; i < sol->card+vns->k && i < problem->n;  i++) 
	//	sol->x[i] = 'a' + GetRandom(problem->sig);

}/* shaking koji dodaje nasumicne karaktere na kraj sekvence koja je vec formirana
 ovo ima posebno smisla kad smo vec dosli do nekog resenja koje je dopustivo */


void shakingInsert(solStruct* sol) {
	int i, j;
	char newx[MAXS];
	/* ubacujemo k elemenata*/
	for (i = 0; i < vns->k; i++)
		vns->v[i] = GetRandom(sol->card + 1 - i);

	variation(sol->card + 1, vns->k, vns->v, vns->v, vns->y);
	for (i = 0; i < problem->n; i++)
		newx[i] = 'a' + problem->sig;
	for (i = 0; i < vns->k; i++)
		newx[vns->v[i]] = 'a' + GetRandom(problem->sig);
	i = 0;
	for (j = 0; j < problem->n && i < sol->card; j++)
		if (!validValue(newx[j]))
			newx[j] = sol->x[i++];

	for (i = 0; i < problem->n; i++)
		sol->x[i] = newx[i];

}/* klasicni shaking sa nasumicnim izmenama */

void shakingSwap(solStruct* sol)
{
	int i, ik, l, r;
	char tmp;
	/* menjamo k elemenata */
	for (i = 0; i < problem->n; i++)
		vns->v[i] = GetRandom(problem->n - i);

	variation(problem->n, vns->k, vns->v, vns->v, vns->y);
	ik = 0;
	l = 0;
	r = problem->n - 1;
	while (ik < vns->k && l < r) {
		while (!validValue(sol->x[vns->v[l]]))
			l++;
		while (!validValue(sol->x[vns->v[r]]) || sol->x[vns->v[r]] == sol->x[vns->v[l]])
			r--;
		//zamenjujemo vrednosti na pozicijama vns->v[l] i vns->v[r]
		tee("%c <--> %c\n", sol->x[vns->v[l]], sol->x[vns->v[r]]);
		tmp = sol->x[vns->v[l]];
		sol->x[vns->v[l]] = sol->x[vns->v[r]];
		sol->x[vns->v[r]] = tmp;
		ik++;
		l++;
		r--;
	}
} /* shaking sa zamenom vrednosti - ima smisla da k ide od 1 ovde */


void LS1bestbestChangeIterationFD(solStruct* sol, int logImprovement) {
	int  i, ir, len, k, besti, oldNonCorrect, oldCard;
	char c, oldc, bestc;
	double newFv, oldFv, bestFv;

	oldFv = sol->fv;
	oldNonCorrect = sol->nonCorrect;
	oldCard = sol->card;
	bestFv = sol->fv;
	if (oldNonCorrect == 0)//ako je resenje vec bez nekorektnih, onda nema nacina da se poboljsa kardinalnost posto se radi samo zamena postojecih karaktera ili njihovo izbacivanje u ovoj metodi
		return;
	for (i = 0; i < problem->n; i++) {
		oldc = sol->x[i];
		//nema potrebe za zamenom praznih mesta, jer je to pokriveno sa InsertLS
		if (!validValue(oldc))
			continue;
		for (c = 'a'; c <= 'a' + problem->sig; c++) {
			if (oldc == c)
				continue;
			sol->x[i] = c;
			newFv = fitness(sol, oldNonCorrect,0);
			if (newFv < bestFv) {
				bestFv = newFv;
				bestc = c;
				besti = i;
			}
		}
		sol->x[i] = oldc;
	}
	if (bestFv < oldFv) {
		sol->x[besti] = bestc;
		sol->fv = fitness(sol, -1,0);
		assert(sol->fv == bestFv);
		if(logImprovement || sol->fv<problem->x.fv)
			tee("Global LSChange besti=%d bestc=%d oldCard=%d newCard=%d oldNonCorrect=%d newNonCorrect=%d\n", besti, bestc, oldCard, sol->card,oldNonCorrect, sol->nonCorrect);
	}
	else {
		sol->fv = fitness(sol, -1,0);
		assert(sol->fv == oldFv);
	}
}


void LS1bestbestChangeIteration(solStruct* sol, int logImprovement) {
	int  i, ir, len, k, besti, oldNonCorrect, oldCard;
	char c, oldc, bestc;
	double newFv, oldFv, bestFv, newFvTest;
	char xTmp[MAXS];

	oldFv = sol->fv;
	oldNonCorrect = sol->nonCorrect;
	oldCard = sol->card;
	bestFv = sol->fv;
	if (oldNonCorrect == 0)//ako je resenje vec bez nekorektnih, onda nema nacina da se poboljsa kardinalnost posto se radi samo zamena postojecih karaktera ili njihovo izbacivanje u ovoj metodi
		return;
	for (i = 0; i < sol->card; i++) {
		//nema potrebe za zamenom praznih mesta, jer je to pokriveno sa InsertLS
		if (!validValue(sol->x[i]))
			continue;
		for (c = 'a'; c <= 'a' + problem->sig; c++) {
			if (sol->x[i] == c)
				continue;
			newFv = fastFitness(sol, oldNonCorrect, i, c, 0);
			
			//provera pocetak
			/*
			for (k = 0; k < problem->n; k++)
				xTmp[k] = sol->x[k];
			//oldc = sol->x[i];
			sol->x[i] = c;
			newFvTest = fitness(sol, -1,0);
			//sol->x[i] = oldc;
			for (k = 0; k < problem->n; k++)
				sol->x[k] = xTmp[k];
			sol->fv = fitness(sol, -1,1);
			assert(sol->fv == oldFv);
			if (newFv != newFvTest) {
			//	sol->x[i] = c;
			//	newFvTest = fitness(sol, -1);
			//	sol->x[i] = oldc;
			//	sol->fv = fitness(sol, -1);
			//	newFv = fastFitness(sol, oldNonCorrect, i, c, 0); 
			}*/
			//provera kraj
			
			if (newFv < bestFv) {
				bestFv = newFv;
				bestc = c;
				besti = i;
			}
		}
	}
	if (bestFv < oldFv) {
		oldc = sol->x[besti];
		sol->x[besti] = bestc;
		sol->fv = fitness(sol, -1,1);
		/*
		if (sol->fv != bestFv) {
			tee("Greskaaaaaaaaa\n");
			sol->x[besti] = oldc;
			sol->fv = fitness(sol, -1);
			newFv = fastFitness(sol, oldNonCorrect, besti, bestc, 0);
		}*/
		assert(sol->fv==bestFv);
		if (logImprovement || sol->fv < problem->x.fv)
			tee("Global LSChange besti=%d bestc=%d oldCard=%d newCard=%d newNonCorrect=%d\n", besti, bestc, oldCard, sol->card, sol->nonCorrect);
	}
}


void LS1randrandChangeIteration(solStruct* sol, int logImprovement) {
	int  i, ix, ir, cr, cx;
	char c, oldc;
	double newFv;

	if (sol->nonCorrect == 0)//ako je resenje vec bez nekorektnih, onda nema nacina da se poboljsa kardinalnost posto se radi samo zamena postojecih karaktera ili njihovo izbacivanje u ovoj metodi
		return;
	ir = GetRandom(problem->n);
	for (ix = 0; ix < problem->n; ix++) {
		i = (ix + ir) % problem->n;
		//nema potrebe za zamenom praznih mesta, jer je to pokriveno sa InsertLS
		if (!validValue(sol->x[i]))
			continue;
		cr = GetRandom(problem->sig+1);
		for (cx = 0; cx <= problem->sig; cx++) {
			c = (cr + cx) % (problem->sig+1) + 'a';
			if (sol->x[i] == c)
				continue;
			newFv = fastFitness(sol, sol->nonCorrect, i, c, 0);
			if (newFv < sol->fv) {
				oldc = sol->x[i];
				sol->x[i] = c;
				sol->fv = fitness(sol, -1, 1);
				//moze biti gornja granica, ali parcijalna funkcija cilja ne sme dobiti bolje resenje od pravog
				//napomena, problem iako maksimizacioni ovde je postavljen kao minimizacioni pa je zato ok da newFv>=sol->fv, to znaci da je parcijalno resenje najvise dobro kao pravo
				/*if (sol->fv < newFv) {
					sol->x[i] = oldc;
					sol->fv = fitness(sol, -1, -1);
					printSolutionDetails(sol);
					newFv = fastFitness(sol, sol->nonCorrect, i, c, 0);
					printf("Test");
				}*/
				assert(sol->fv <= newFv);
				if (logImprovement || sol->fv < problem->x.fv)
					tee("Global LSChange besti=%d bestc=%d newCard=%d newNonCorrect=%d\n", i, c, sol->card, sol->nonCorrect);
				return;
			}
		}
	}
}

void LS1bestbestInsertIteration(solStruct* sol, int logImprovement) {
	int i, j, ir, sr, sx, s, len, solSize, besti;
	char c, bestc;
	double newFv, oldFv, bestFv, tmpFv;
	int oldNonCorrect;
	int oldCard;
	static char* arr = NULL;
	if (sol->nonCorrect > 0)
		return;
	if (arr == NULL)
		arr = (char*)malloc(sizeof(char) * problem->n);
	solSize = 0;
	for (i = 0; i < problem->n; i++) {
		if (validValue(sol->x[i]))
			arr[solSize++] = sol->x[i];
	}
	if (solSize == problem->n)
		return;
	oldFv = sol->fv;
	oldNonCorrect = sol->nonCorrect;
	oldCard = sol->card;
	bestFv = sol->fv;
	for (i = 0; i <= solSize; i++) {
		for (c = 'a'; c < 'a' + problem->sig; c++) {
			newFv = fastFitness(sol, oldNonCorrect,i,c, 1);
			if (newFv < bestFv) {
				bestFv = newFv;
				bestc = c;
				besti = i;
			}
		}
	}

	if (bestFv < oldFv) {
		//tee("Novo poboljsanje %.2lf u odnosu na %.2lf\n", bestFv, oldFv);
		for (j = 0; j < besti; j++)
			sol->x[j] = arr[j];
		for (j = besti; j < solSize; j++)
			sol->x[j + 1] = arr[j];
		for (j = solSize + 1; j < problem->n; j++)
			sol->x[j] = 'a' + problem->sig;
		sol->x[besti] = bestc;
		sol->fv = fitness(sol, -1,1);
		assert(sol->fv == bestFv);
		if (logImprovement || sol->fv < problem->x.fv)
			tee("Global LSInsert besti=%d bestc=%d oldCard=%d newCard=%d newNonCorrect=%d\n", besti, bestc, oldCard, sol->card, sol->nonCorrect);
	}
}


void LS1randrandInsertIteration(solStruct* sol, int logImprovement) {
	int  i, j,ix, ir, cr, cx, solSize;
	char c;
	double newFv;
	static char* arr = NULL;
	if (sol->nonCorrect > 0)
		return;
	if (arr == NULL)
		arr = (char*)malloc(sizeof(char) * problem->n);
	solSize = 0;
	for (i = 0; i < problem->n; i++) {
		if (validValue(sol->x[i]))
			arr[solSize++] = sol->x[i];
	}
	if (solSize == problem->n)
		return;
	ir = GetRandom(solSize+1);
	for (ix = 0; ix <= solSize; ix++) {
		i = (ix + ir) % (solSize + 1);
		cr = GetRandom(problem->sig);
		for (cx = 0; cx < problem->sig; cx++) {
			c = (cr + cx) % (problem->sig) + 'a';
			newFv = fastFitness(sol, sol->nonCorrect, i, c, 1);
			if (newFv < sol->fv) {
				for (j = 0; j < i; j++)
					sol->x[j] = arr[j];
				for (j = i; j < solSize; j++)
					sol->x[j + 1] = arr[j];
				for (j = solSize + 1; j < problem->n; j++)
					sol->x[j] = 'a' + problem->sig;
				sol->x[i] = c;
				sol->fv = fitness(sol, -1, 1);
				assert(sol->fv == newFv);
				if (logImprovement || sol->fv < problem->x.fv)
					tee("Global LSInsert besti=%d bestc=%d newCard=%d newNonCorrect=%d\n", i, c,sol->card, sol->nonCorrect);
				return;
			}
		}
	}
}

int notFinished() {
	return vns->iter < vns->iterMax&& vns->iter - vns->iterp < vns->iterWoChange && ((double)clock() - vns->runstart) / CLOCKS_PER_SEC < vns->maxTime;
}

void LS1(solStruct *sol, int logImprovement) {
	double oldFv;
	int impr = 1;
	while (impr && notFinished()) {
		impr = 0;
		oldFv = sol->fv;
		LS1randrandChangeIteration(sol, logImprovement);
		if (sol->fv < oldFv) {
			impr = 1;
			continue;
		}
		LS1randrandInsertIteration(sol, logImprovement);
		if (sol->fv < oldFv) {
			impr = 1;
			continue;
		}
	}
}


void LS1Insert(solStruct* sol, int logImprovement) {
	double oldFv;
	int impr = 1;
	while (impr) {
		impr = 0;
		oldFv = sol->fv;
		LS1bestbestInsertIteration(sol, logImprovement);
		if (sol->fv < oldFv) {
			impr = 1;
			continue;
		}
	}
}


int mainIteration()
{
	int i, same;
	double ls1fv;
	double oldFv;

	if (vns->k >= problem->n) return 0;

	vns->iter++;

	problem->x.fv = fitness(&problem->x, -1,1);

	/* kopiramo */
	copy(&problem->xt, &problem->x);

	if (problem->xt.nonCorrect > 0) 
		shakingChange(&problem->xt);
	else 
		shakingDelete(&problem->xt);

	problem->xt.fv = fitness(&problem->xt,-1,1);

	LS1(&problem->xt,0);

	if (problem->xt.fv < problem->x.fv) {
		copyt();
		return 1;
	}
	if (problem->xt.fv > problem->x.fv)
		return 0;

	return Flip(vns->prob);
} /* glavna iteracija */

int startsWith(const char* pre, const char* str)
{
	size_t lenpre = strlen(pre);
	size_t lenstr = strlen(str);
	return lenstr < lenpre ? 0 : memcmp(pre, str, lenpre) == 0;
}

void parameterError(const char* paramName) {
	printf("Unrecognized parameter or incorrect format: %s\n", paramName);
	printf("Given command line: %s\n", vns->cmdl);
	printf("Expected usage: .out/.exe -fname ? -kmin ? -kmax ? -itermax ? -iterwochange ? -maxtime ? -prob ? -seed ? -irace ?(0/1)\n");
	exit(EXIT_FAILURE);
}

void getParam(const char* paramName,char* paramVal,char** argv, int argc) {
	int i;
	for (i = 0; i < argc; i++) {
		if(strcmp(argv[i],paramName)==0){
			if (i + 1 >= argc)
				parameterError(paramName);
			strcpy(paramVal, argv[i + 1]);
			return;
		}
	}
	parameterError(paramName);
}

int main(int argc, char** argv)
{
	int i;
	char paramVal[MAXS];
	vns = (vnsStruct*)malloc(sizeof(vnsStruct));
	problem = (problemStruct*)malloc(sizeof(problemStruct));

	vns->runstart = clock();
	time(&vns->tstart);

	vns->cmdl[0] = '\0';
	for (i = 0; i < argc; i++)
	{
		strcat(vns->cmdl, argv[i]);
		strcat(vns->cmdl, " ");
	}

	getParam("-fname", paramVal, argv, argc);
	sprintf(vns->imeul, "%s", paramVal);
	sprintf(vns->imeInit, "%s.sol", paramVal);
	getParam("-kmin", paramVal, argv, argc);
	vns->kmin = atoi(paramVal);
	getParam("-kmax", paramVal, argv, argc);
	vns->kmaxinit = atoi(paramVal);
	getParam("-itermax", paramVal, argv, argc);
	vns->iterMax = atoi(paramVal);
	getParam("-iterwochange", paramVal, argv, argc);
	vns->iterWoChange = atoi(paramVal);
	getParam("-maxtime", paramVal, argv, argc);
	vns->maxTime = atoi(paramVal);
	getParam("-prob", paramVal, argv, argc);
	vns->prob = atof(paramVal);
	getParam("-seed", paramVal, argv, argc);
	vns->seed = atoi(paramVal);
	getParam("-irace", paramVal, argv, argc);
	vns->iraceMode = atoi(paramVal);
	getParam("-fname", paramVal, argv, argc);
	sprintf(vns->imeout, "%s_%d_%d_%d_%d_%d_%.2f_%d.out", paramVal, vns->kmin,vns->kmaxinit,vns->iterMax,vns->iterWoChange,vns->maxTime,vns->prob,vns->seed);
	sprintf(vns->imelog, "%s_%d_%d_%d_%d_%d_%.2f_%d.log", paramVal, vns->kmin, vns->kmaxinit, vns->iterMax, vns->iterWoChange, vns->maxTime, vns->prob, vns->seed);

	SetRandomSeed(vns->seed);
	input();
	init();
	while (notFinished())
	{
		//if (vns->iter % 1000 == 0)
		tee("t=%.1lf iter=%d k=%d vr=%g nonCorrect=%d card=%d maxChecks=%d realChecks=%d\n", ((double)(clock() - vns->runstart)) / CLOCKS_PER_SEC, vns->iter, vns->k, problem->x.fv, problem->x.nonCorrect, problem->x.card, fitMaxChecks,fitRealChecks);
		//printSolutionDetails(&problem->x);
		if (mainIteration()) {
			copy(&problem->x, &problem->xt);
			vns->k = vns->kmin;
		}
		else if (vns->k < vns->kmax)
			vns->k++;
		else
			vns->k = vns->kmin;
	}
	vns->fv = problem->x.fv;
	stopTime();
	fitness(&problem->x,-1,1);
	//validate();
	if (vns->iraceMode)
		printf("val: %d\ntime: %.4f\n",problem->x.fv<1?problem->x.card:0,vns->rt);
	else
		printAll();
	//aktivirati ponovo dealokaciju - postoji neki problem sa Hipom
	deallocation();
	return 0;
}


/******************
Pomocne funkcije
******************/

/* generise varijaciju v bez ponavljanja sa k elemenata od n mogucih (ako je n=k permutacija) generisanu sa g */
int variation(int n, int k, int* g, int* v, int* y)
{
	int i, j, p;

	if (y == NULL || g == NULL || v == NULL) return 0; /* nizovi nisu alocirani */

	for (i = 0; i < n; i++)
		y[i] = 0;

	for (i = 0; i < k; i++)
	{
		if (g[i] < 0 || g[i] >= n - i) return 0;  /* generator g je izvan opsega */
		for (j = 0, p = 0; j <= g[i]; j++, p++)
			while (y[p]) p++;
		v[i] = p - 1;
		y[p - 1] = 1;
	}
	return 1;  /* sve je u redu */
}

