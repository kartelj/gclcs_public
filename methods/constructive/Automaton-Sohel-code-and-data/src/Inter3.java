
import java.util.*;

public class Inter3 {
	public int sigma;
	HashMap maps = new HashMap();
//int din[]=new int[interSize];
	static int intersectCnt = 0;
	int restrictStr;
	boolean cin[] = new boolean[Main.maxInterSize];
	int myDelta1[][];
	int states[][] = new int[Main.maxInterSize][72];
	Queue2 Qnew = new Queue2();
	int count2;
	String str;
	Key k2 = new Key();
	HashMap index = new HashMap();
	char[] sigmaSet;

	Inter3(char[] sigmaSet) {
		this.sigmaSet = sigmaSet;
		this.sigma = sigmaSet.length;
		myDelta1=new int[Main.maxInterSize][sigma];
		for (int i = 0; i < sigma; i++) {
			index.put((Character) sigmaSet[i], index.size());
		}

	}

	Inter3() {
		this(new char[] { 'A', 'C', 'G', 'T', 'D', 'E', 'F', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'V', 'W',
				'Y' });
	}

	DFA2 SuperSequence(char[] str) {
		int n = str.length, i, j;
		HashSet final1 = new HashSet();
		int myDelta[][] = new int[n + 2][sigma];
		DFA2 d1 = new DFA2();

		d1.startState = 1;
		d1.stateNo = n + 1;
		for (i = 1; i <= n; i++) {
			myDelta[i][(Integer) index.get(str[i - 1])] = i + 1;
			for (j = 0; j < sigma; j++)
				if (sigmaSet[j] != str[i - 1])
					myDelta[i][(Integer) index.get(sigmaSet[j])] = i;

		}
		for (j = 0; j < sigma; j++)
			myDelta[n + 1][(Integer) index.get(sigmaSet[j])] = n + 1;

		final1.add(n + 1);

		d1.sigma = sigmaSet;
		d1.delta = myDelta;
		d1.accept = final1;
		return d1;
	}

	DFA3 InterSection6(DFA2 M1, DFA2 M2) {
		intersectCnt++;
		HashSet fnl = new HashSet(), m1 = M1.accept;
		boolean acc[] = new boolean[Main.maxInterSize];

		DFA3 d = new DFA3();
		for (int[] row : states)
			Arrays.fill(row, -1);
		for (int[] row : myDelta1)
			Arrays.fill(row, -1);
		boolean flag = false;
		if (intersectCnt == this.restrictStr)
			flag = true;

		int p1, p2, p1p2, q1q2, i, m2st = M2.stateNo - 1, count = 0, rlen, j = 0;
		states[M1.startState][M2.startState] = count++;
		fnl.add(0);
		acc[0] = true;

		Key k;
		j = 0;
		Qnew.init();// q1=k.first; q2=k.last;
		Qnew.enqueue(M1.startState, M2.startState);
		while (Qnew.size > 0) {
			k = Qnew.dequeue();
			q1q2 = states[k.first][k.last];
			cin[q1q2] = false;
			for (i = 0; i < sigma; i++) {
				p1 = M1.delta[k.first][i]; // p1= -1 if no transition for sigma[i]
				p2 = M2.delta[k.last][i]; // p2= -1 if no transition for sigma[i]

				if ((m1.contains(p1))) {
					if (states[p1][p2] == -1)
						states[p1][p2] = count++;
					// System.out.println(j++ +" p2+ "+p1+" "+p2);

					p1p2 = states[p1][p2];
					myDelta1[q1q2][i] = p1p2;//
					if (!cin[p1p2]) {
						Qnew.enqueue(p1, p2);
						cin[p1p2] = true;
						if ((M2.accept.contains(p2))) {// !fnl.contains(p1p2)&
							// fnl.add(p1p2);//System.out.println(" p2 val "+p2 );
							acc[p1p2] = true;
						}
					}
				}
			}

		}
		// System.out.println(" 6 "+intersectCnt+flag );
		if (restrictStr > 1) {
			int[][] newArray = (int[][]) myDelta1.clone();
			for (int row = 0; row < count; row++) {
				newArray[row] = (int[]) myDelta1[row].clone();
			}
			d.stateNo = count;
			d.delta = newArray;
			d.sigma = M1.sigma;
			d.startState = 0;
			d.accept = acc;
			return d;

		}

		else {
			d.stateNo = count;
			d.delta = myDelta1;
			d.sigma = M1.sigma;
			d.startState = 0;
			d.accept = acc;
			return d;
		}

	}

	DFA3 InterSection7(DFA3 M1, DFA2 M2) {
		HashSet fl = new HashSet();
		boolean acc[] = new boolean[Main.maxInterSize];
		intersectCnt++;// DFA d=new DFA();
		for (int[] row : states)
			Arrays.fill(row, -1);

		int p1, p2, p1p2, q1q2, i, rlen, count = 0, m2st = M2.stateNo - 1;
		boolean flag = false;
		states[M1.startState][M2.startState] = count++;
		fl.add(0);
		acc[0] = true;

		if (intersectCnt == this.restrictStr)
			flag = true;

		for (int[] row : myDelta1)
			Arrays.fill(row, -1);

		Arrays.fill(cin, false);
		Key k;
		Qnew.init();
		Qnew.enqueue(M1.startState, M2.startState);
		while (Qnew.size > 0) {
			k = Qnew.dequeue();
			q1q2 = states[k.first][k.last];
			cin[q1q2] = false;
			for (i = 0; i < sigma; i++) {
				p1 = M1.delta[k.first][i]; // p1= -1 if no transition for sigma[i]
				p2 = M2.delta[k.last][i]; // p2= -1 if no transition for sigma[i]
				if (p1 > -1) {// -1
					if (states[p1][p2] == -1)
						states[p1][p2] = count++;
					p1p2 = states[p1][p2];
					myDelta1[q1q2][i] = p1p2;
					if (!cin[p1p2]) {
						Qnew.enqueue(p1, p2);
						cin[p1p2] = true;//
						if (M1.accept[p1] && (M2.accept.contains(p2)))
							// fl.add(p1p2);
							acc[p1p2] = true;
					}
				}
			}

		}

		if (flag == false) {
			int[][] newArray = (int[][]) myDelta1.clone();
			rlen = count;
			for (int row = 0; row < rlen; row++) {
				newArray[row] = (int[]) myDelta1[row].clone();
			}
			DFA3 d1 = new DFA3(count, 0, M1.sigma, newArray, acc);
			// System.out.println(" 7 "+intersectCnt+" "+newArray.length );
			return d1;

		}

		else {
			DFA3 d = new DFA3(count, 0, M1.sigma, myDelta1, acc);// (a1, start, M1.sigma, myDelta1, final1);
			/*
			 * d.stateNo=count; d.delta=myDelta1; d.sigma=M1.sigma; d.startState=0;
			 * d.accept=fl;
			 */
			return d;
		}

	}

}
