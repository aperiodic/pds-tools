import java.util.Map;
import java.util.LinkedHashMap;

public class PDSTools {
  public native Map<String,String> parse(String filename, Map<String,String> instance);

  public static void main(String[] args) {
    String label_name;
    if (args.length > 1) {
      label_name = args[0];
    } else {
      label_name = "samples/mola.lbl";
    }

    System.loadLibrary("pdstools");
    PDSTools pds_tools = new PDSTools();
    Map<String,String> blank = new LinkedHashMap<String,String>();
    Map<String,String> label = pds_tools.parse(label_name, blank);

    System.out.println("parsed " + label_name);
  }
}
