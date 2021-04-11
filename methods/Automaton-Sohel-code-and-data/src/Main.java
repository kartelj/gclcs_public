import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

public class Main  {

	public static final int maxQueueSize=10000000;
	public static final int maxInterSize =5000000;
			
	public static void main(String args[]) {

		if (args.length != 3) {

			System.out.println(
					"Incorrect usage: [fileInputPath] [fileOutputPath] [problemMethod type: 0: \"Seq-Ic-LCS FA\", 1: \"Seq-Ic-LCS CC\", 2: \"Seq-Ec-LCS FA\", 3: \"Seq-Ec-LCS ZG\", 4: \"Str-Ec-LCS FA\", 5:\"Str-Ec-LCS HYA\"");
			System.exit(1);
		}
		String fileInputPath = args[0];
		Reader reader = new Reader();
		reader.readInput(fileInputPath);
		reader.run();
		var sigma = new char[reader.readSigma.size()];
		int k=0; 
		for(var c : reader.readSigma) 
			sigma[k++]=c;
		
		String fileOutputPath = args[1];
		Writer writer = null;
		try {
			writer = new FileWriter(new File(fileOutputPath));
		} catch (IOException e) {
			System.out.println("Errro when openning output file "+fileOutputPath);
			System.exit(1);
		}
		

		int problemMethod = Integer.parseInt(args[2]);
		FiniteAutomata F = null;
		
		switch (problemMethod) {
			case 0:
				F = new FiniteAutomata();
				//TODO ACA: I've adjusted Seq_Ic to work with arbitrary input alphabet - something similiar still needs to be done for other methodProblem types
				F.Seq_Ic(reader, writer, sigma);
				break;
			case 1:
				SeqIc r = new SeqIc();
				// r.DPmethod2(reader.inputStr);
				if (reader.totalStr.size() == 3)
					r.DP1(reader.totalStr);
				else if (reader.totalStr.size() == 4)
					r.DP2(reader.totalStr);
				else if (reader.totalStr.size() == 5)
					r.DP3(reader.totalStr);
				else
					r.DP4(reader.totalStr);
				break;
			case 2:
				F = new FiniteAutomata();
				F.Seq_Ec(reader, sigma);
				break;
			case 3:
				RLCS rlcs = new RLCS();
				if (reader.totalStr.size() == 3)
					rlcs.DPmethod2(reader.totalStr);
				else if (reader.totalStr.size() == 4)
					rlcs.DPmethod3(reader.totalStr);
				else if (reader.totalStr.size() == 5)
					rlcs.DPmethod5_32(reader.totalStr);
				else
					rlcs.DPmethod(reader.totalStr);
				break;
			case 4:
				F = new FiniteAutomata();
				F.Str_Ec(reader, sigma);
				break;
			case 5:
				StrEc rstrec = new StrEc();
				if (reader.totalStr.size() == 3)
					rstrec.StrAlgo1(reader.totalStr);
				else if (reader.totalStr.size() == 4)
					rstrec.StrAlgo2(reader.totalStr);
				else if (reader.totalStr.size() == 5)
					rstrec.StrAlgo3(reader.totalStr);
				else
					rstrec.StrAlgo4(reader.totalStr);
				break;
			default:
				System.out.println("Problem method type not recognized, the value should be from {0, 1, ..., 5}");
				System.exit(1);
		}
	}
}
