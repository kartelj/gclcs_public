import java.io.IOException;
import java.io.Writer;

public class FiniteAutomata {
	
	void Seq_Ic(Reader reader, Writer writer, char[] sigma) {

		SA98 sa = new SA98(sigma);
		DFA2 d4;
		DFA3 d22 = null;
		DFA d3;
		Inter3 test = new Inter3(sigma);

		int i, strCount, j, restrictCnt;
		String concatStr = reader.concatInput();
		strCount = reader.inputStr.size();

		test.restrictStr = reader.pattern.size();
		restrictCnt = test.restrictStr;
		long startTime = System.currentTimeMillis();

		sa.construct(concatStr.toCharArray(), strCount);
		d4 = test.SuperSequence(((String) reader.pattern.get(0)).toCharArray());
		d22 = test.InterSection6(sa.d, d4);
		for (i = 1; i < restrictCnt; i++)
			d22 = test.InterSection7(d22, test.SuperSequence(((String) reader.pattern.get(i)).toCharArray()));
		MaxFA Mx = new MaxFA(d22.stateNo, sigma);
		d3 = Mx.MaxAutomata(d22);
		
		long endTime = System.currentTimeMillis();// System.currentTimeMillis();System.nanoTime();
		System.out.println("value: "+d3.result);
		System.out.println("time: "+(endTime-startTime)/1000.0);
		try {
			writer.write("value: "+d3.result+System.lineSeparator());
			writer.write("time: "+(endTime-startTime)/1000.0);
			writer.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//System.out.println("Total elapsed time in execution of DFA2 CLCS  is :" + (endTime - startTime));
		//System.out.println("Total elapsed time in execution of DFA2 CLCS  is :" + (endTime - startTime));

		//System.out.print("common SA state " + sa.Q.size());
		//System.out.println("   total SA state " + sa.stCount);
		//System.out.print("input str " + concatStr.length());
		// reader.out.close();
		//System.out.println("intersect state" + d22.stateNo);
		//System.out.println("maxlen states " + d3.stateNo.size() + " states");
	}

	void Seq_Ec(Reader reader, char[] sigma) {
		SA99 sa = new SA99();
		DFA2 d4;
		DFA3 d22 = null;
		DFA d3;
		InterSeqEc test = new InterSeqEc();
		// Max Mx=new Max();

		int i, strCount, j;
		String concatStr = reader.concatInput();
		strCount = reader.inputStr.size();

		long startTime = System.currentTimeMillis();
		test.restrictStr = reader.pattern.size();
		sa.construct(concatStr.toCharArray(), strCount);

		d4 = test.SupeSequence(((String) reader.pattern.get(0)).toCharArray());
		d22 = test.InterSection6(sa.d, d4);
		for (i = 1; i < test.restrictStr; i++)
			d22 = test.InterSection7(d22, test.SupeSequence(((String) reader.pattern.get(i)).toCharArray()));
		MaxFA Mx = new MaxFA(d22.stateNo, sigma);
		d3 = Mx.MaxAutomata(d22);

		long endTime = System.currentTimeMillis();// System.currentTimeMillis();System.nanoTime();

		System.out.println("Total elapsed time in execution of DFA2 CLCS  is :" + (endTime - startTime));
		System.out.print("common SA state " + sa.Q.size());
		System.out.println("   total SA state " + sa.stCount);
		System.out.print("input str " + concatStr.length());
		// reader.out.close();
		System.out.println("intersect state" + d22.stateNo);
		System.out.println("maxlen states " + d3.stateNo.size() + " states");
	}

	void Str_Ec(Reader reader, char[] sigma) {
		long startTime = System.currentTimeMillis();
		SAStrEc sa = new SAStrEc();
		DFA2 d4;
		DFA3 d22 = null;
		DFA d3;
		InterStr test = new InterStr();
		// Max Mx=new Max();

		int i, strCount, j;
		String concatStr = reader.concatInput();
		strCount = reader.inputStr.size();

		test.restrictStr = reader.pattern.size();
		sa.construct(concatStr.toCharArray(), strCount);

		d4 = test.Superstr(((String) reader.pattern.get(0)).toCharArray());
		d22 = test.InterSection6(sa.d, d4);
		for (i = 1; i < test.restrictStr; i++)
			d22 = test.InterSection7(d22, test.Superstr(((String) reader.pattern.get(i)).toCharArray()));
		// Mx.din=test.din;SuperSequence Superstr2
		MaxFA Mx = new MaxFA(d22.stateNo, sigma);
		d3 = Mx.MaxAutomata(d22);

		long endTime = System.currentTimeMillis();// System.currentTimeMillis();System.nanoTime();
		
		System.out.println("Total elapsed time in execution of DFA2 CLCS  is :" + (endTime - startTime));
		// d3=Mx.MaxAutomata(d22);
		/*--------Output of DFA---------------------------------------------*/
		System.out.println("Total elapsed time in execution of DFA2 CLCS  is :" + (endTime - startTime));

		System.out.print("common SA state " + sa.Q.size());
		System.out.println("   total SA state " + sa.stCount);
		// reader.out.close();
		System.out.println("intersect state" + d22.stateNo);
		System.out.println("maxlen states " + d3.stateNo.size() + " states");
	}
}
