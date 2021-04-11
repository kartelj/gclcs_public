
import java.util.*;

public class MaxFA {

	public static int maxLen;// 20000 protease--165888 else 120888
	public int sigmaLen;
	int din[];// maps.size
	ArrayList[] delR;
	int len[];
	int newDelta[][];// =new int[maxLen][sigmaLen];
	boolean Qin[];

	MaxFA(int L, char[] sigmaSet) {
		sigmaLen = sigmaSet.length;
		maxLen = L;
		int i, j;

		din = new int[maxLen];// maps.size
		delR = new ArrayList[maxLen];
		len = new int[maxLen];

		Qin = new boolean[maxLen];
		for (i = 0; i < maxLen; i++) {
			// for(j=0;j<sigmaLen ;j++)
			delR[i] = new ArrayList();
		}
		Arrays.fill(len, -999);

	}

	DFA MaxAutomata(DFA3 M) {
		int p, q = 0;
		int lens = 0;
		HashSet starts = new HashSet();
		int i, j, s;
		// HashSet Q2=new HashSet(M.accept);
		Queue removStState = new LinkedList();

		DFA d = new DFA();
		if (M.stateNo > maxLen)
			for (i = maxLen; i < M.stateNo; i++) {
				// for(j=0;j<sigmaLen ;j++)
				delR[i] = new ArrayList();
			}
		len[M.startState] = 0;
		int statesNo = M.stateNo;
		for (i = 0; i < statesNo; i++)
			for (j = 0; j < sigmaLen; j++)
				if (M.delta[i][j] != -1)
					din[M.delta[i][j]] += 1;
		Queue C = new LinkedList();
		// C.init();
		C.add(0);
		while (C.size() > 0) {
			p = ((Integer) C.remove());
			for (s = 0; s < sigmaLen; s++) {// s=i;
				q = M.delta[p][s];
				if (q == -1 || s == -1) {
					// System.out.println(" q=-1 "+q+" "+p+" "+s);
					continue;// if(s==-1) continue;
				}
				if (q > -1)
					din[q] -= 1;
				if (din[q] == 0)
					C.add(q);
				if (len[q] < len[p] + 1) {
					len[q] = len[p] + 1;
					// if(delR[q].size()==0)
					// delR[q]=new ArrayList();
					delR[q].clear();
					delR[q].add(0, s);
					delR[q].add(p);
				} else if (len[q] == len[p] + 1)
					delR[q].add(p);
				// Check must ---
				// if(Q2.contains(q))
				if (M.accept[q]) {
					if (lens < len[q]) {
						lens = len[q];
						starts.clear(); // removStState.clear();
						starts.add(q);// removStState.add(q);
					} else if (lens == len[q] && !starts.contains(q)) {
						starts.add(q);// removStState.add(q);
					}
				}
			}
		} // -------------remov state
		HashSet Q1 = new HashSet();
		Iterator it;// for(i=0;i<starts.size();i++)
		it = starts.iterator();
		while (it.hasNext())
			removStState.add((Integer) it.next());
		while (!removStState.isEmpty()) {// removStState.add(((Integer)starts.get(i)).intValue());
			p = ((Integer) removStState.remove());
			if (!Qin[p]) {
				Q1.add(p);
				Qin[p] = true;
			}
			// for(i=0; i<sigmaLen ; i++){
			// it =delR[p].iterator();
			// q=(Integer)it.next();// index
			// while(it.hasNext())
			for (i = 1; i < delR[p].size(); i++) {
				q = (Integer) delR[p].get(i); // (Integer)it.next();
				if (!Qin[q]) {
					removStState.add(q);
					Q1.add(q);
					Qin[q] = true;
				}
			}
			// }
		} // # states==Q.size //-------------reverse

		newDelta = new int[maxLen][sigmaLen];
		Iterator it1 = Q1.iterator();// for(i=0;i<Q1.size();i++)
		while (it1.hasNext()) {
			p = (Integer) it1.next();
			// for(j=0;j< sigmaLen ; j++){
			// it =delR[p].iterator();
			if (delR[p].size() > 1) {
				j = (Integer) delR[p].get(0); // it.next();
				// while(it.hasNext())
				for (i = 1; i < delR[p].size(); i++) {
					q = (Integer) delR[p].get(i);
					if (Qin[q])
						newDelta[q][j] = p;
				}
			}
		}

		d.stateNo = new ArrayList(Q1);
		d.delta = newDelta;
		d.sigma = M.sigma;
		d.startState = 0;
		d.accept = new ArrayList(starts);
		d.result = lens;
		// System.out.println("Total length DFA CLCS is :"+ lens);

		return d;
	}

}
