
import java.util.*;

public class SA98 implements Cloneable {

	// infty=40000
	public static final int Infty = 390048;// 29948
	public static final int maxSize = 390050;// 29950
	HashSet[] NonSolidArc = new HashSet[maxSize];
	int sigma;
	HashSet[] NIfty;
	// BitSet[][]Ns= new BitSet [35000][sigma];
	HashSet[] targetSt;

	HashSet Q = new HashSet();
	ArrayList lastTargtSt;
	int count;
	int Din[] = new int[maxSize];
	int myDelta[][];
	int Match[] = new int[maxSize];
	DFA2 d;
	int totalInput = 0;

	int stCount = 0;

	SA98(char[] sigmaSet) {
		sigma = sigmaSet.length;
		NIfty= new HashSet[sigma];
		targetSt= new HashSet[sigma];
		myDelta= new int[maxSize][sigma];
		d = new DFA2();
		int i, j, idx;
		for (i = 0; i < sigma; i++)
			d.addSymbol(sigmaSet[i]);
		d.sigma = sigmaSet;
		d.startState = 0;
		for (i = 0; i < sigma; i++)
			targetSt[i] = new HashSet();

		for (i = 0; i < maxSize; i++) {
			// lastNonSolidArc[i]=new HashSet();
			// for(j=0;j<sigma;j++)
			NonSolidArc[i] = new HashSet(10);
		}

		for (j = 0; j < sigma; j++)
			NIfty[j] = new HashSet();
		for (i = 0; i < sigma; i++) {//
			idx = d.idxSymbol(d.sigma[i]);
			myDelta[0][idx] = myDelta[Infty][idx] = Infty;
			NIfty[idx].add(0);
			targetSt[idx].add(Infty);
		}
		Match[0] = 1;
		Match[Infty] = 0;
		Q.add(0);
		stCount++;
		// System.out.println("d map "+d.map.size());
	}

	SA98() {
		this(new char[] { 'A', 'C', 'G', 'T', 'D', 'E', 'F', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'V', 'W',
				'Y' });
	}

	void extendDim() { // idx=d.idxSymbol(d.sigma[i]);//length=targetSt.get(idx).size();
		int r, j, i;// d.sigma.length
		Iterator it;
		for (i = 0; i < sigma; i++) {
			it = targetSt[i].iterator();
			while (it.hasNext()) {// {//j=(Integer)it.next();
				j = (Integer) it.next();
				if (j != Infty)
					NonSolidArc[j] = new HashSet(10);
				else if (j == Infty)
					NIfty[i] = new HashSet(10);
			}
			r = myDelta[0][i];
			// check
			if (r != Infty) {
				NonSolidArc[r] = new HashSet();
				// NonSolidArc[r][i]=new HashSet();
				NonSolidArc[r].add(0);
			} else if (r == Infty) {
				NIfty[i] = new HashSet();
				NIfty[i].add(0);
			}
			targetSt[i] = new HashSet(10);
			targetSt[i].add(r);
		}
		Match[0]++;// if((Match[0]==totalInput)&&!Q.contains(0))Q.add(0);

	}

	void appendChar(char c) {
		ArrayList[] lastNonSolidArc = new ArrayList[maxSize];
		// System.out.println("d map "+c);
		int idx = d.idxSymbol(c), len, p, k, r, r1, i, j, i1, i2, j1, a, length, it2;
		lastTargtSt = new ArrayList(targetSt[idx]);
		targetSt[idx] = new HashSet();
		Iterator it;
		length = lastTargtSt.size();
		for (i = 0; i < length; i++) {
			i1 = (Integer) lastTargtSt.get(i);
			boolean isIfty = false;
			if (i1 == Infty)
				isIfty = true;
			if (!isIfty)
				it = NonSolidArc[i1].iterator();
			else
				it = NIfty[idx].iterator();
			lastNonSolidArc[i1] = new ArrayList();
			while (it.hasNext())
				lastNonSolidArc[i1].add(it.next());
			// Check
			if (!isIfty)
				NonSolidArc[i1] = new HashSet();
			// =new HashSet();
			else
				NIfty[idx] = new HashSet();
			// =new HashSet();
			// NonSolidArc[i1][idx]=new HashSet();
		}
		for (i = 0; i < length; i++) {
			i1 = (Integer) lastTargtSt.get(i);
			boolean isIfty = false;
			if (i1 == Infty)
				isIfty = true;
			if ((lastNonSolidArc[i1].size() == Din[i1]) & (i1 != Infty))
				p = i1;
			else
				p = stCount++;// maps.put(p, p);
			for (a = 0; a < sigma; a++) {
				r = myDelta[i1][a];
				if ((r1 = myDelta[p][a]) > 0)
					Din[r1]--;
				myDelta[p][a] = r;
				if (r > 0) {
					Din[r]++;
					if (!targetSt[a].contains(r))
						targetSt[a].add(r);
				}
				if (!isIfty) {
					if ((r > 0) && (!NonSolidArc[r].contains(p)))
						NonSolidArc[r].add(p);
				} else if (!NIfty[a].contains(p))
					NIfty[a].add(p);
			}
			Match[p] = Match[i1] + 1;
			if ((Match[p] == totalInput))
				Q.add(p);
			it = lastNonSolidArc[i1].iterator();
			while (it.hasNext()) {
				j1 = (Integer) it.next();
				if ((r1 = myDelta[j1][idx]) > 0)
					Din[r1]--;
				myDelta[j1][idx] = p;
				Din[p]++; // System.out.println("Din "+r1+" "+Din[r1]+" "+p+" "+Din[p]);
			}
			lastNonSolidArc[i1].clear();
		}
	}

	void construct(char[] cArray, int strCnt) {
		totalInput = strCnt;

		int len = cArray.length, i;
		for (i = 0; i < len; i++) {
			if (cArray[i] == '#') {
				extendDim();
			} else
				appendChar(cArray[i]);
		}

		/************************************************************************/
		HashMap SAstates = new HashMap();
		HashSet Q1 = new HashSet();
		Q1.add(0);
		int myDelta1[][] = new int[Q.size() + 2][sigma];
		for (int[] row : myDelta1)
			Arrays.fill(row, -1);
		SAstates.put(0, new Integer(SAstates.size()));
		// for(i=0;i<sigma;i++)
		// myDelta1[0][i]=myDelta[0][i];
		Iterator QtempIt = Q.iterator();
		while (QtempIt.hasNext()) {
			int temp = (Integer) (QtempIt.next()), temp2, temp3;
			if (temp != 0) {
				temp2 = (Integer) (SAstates.size());
				SAstates.put(temp, temp2);

				// System.out.println("map "+temp+" "+temp2);
				Q1.add(temp2);
			}
		}

		QtempIt = Q.iterator();
		while (QtempIt.hasNext()) {
			int temp = (Integer) (QtempIt.next()), temp2, temp3;
			temp2 = (Integer) (SAstates.get(temp));

			for (i = 0; i < sigma; i++) {
				temp3 = myDelta[temp][i];
				if (!(Q.contains(temp3)))
					myDelta1[temp2][i] = Infty;
				else
					myDelta1[temp2][i] = (Integer) (SAstates.get(temp3));
				// System.out.println("delta "+temp3+" delta1 "+myDelta1[temp2][i]);

			}
		}

		d.delta = myDelta1;
		d.stateNo = Q1.size();
		d.accept = Q1;
	}

}
