package org.aperiodic;

import java.util.Map;
import java.util.LinkedHashMap;

public class PDSTools {
  public native Map<String,Object> parse(String filename);
  private static boolean loaded = false;

  public static PDSLabel parseLabel(String filename) {
    if (!loaded) {
      System.loadLibrary("pdstools");
      loaded = true;
    }

    PDSTools jni_wrapper = new PDSTools();
    Map<String,Object> mlabel = jni_wrapper.parse(filename);

    if (mlabel == null) {
      return null;
    }

    String version = (String) mlabel.get("PDS_VERSION_ID");
    Map<String,Object> metadata = (Map<String,Object>) mlabel.get("METADATA");
    Map<String,Map<String,Object>> objects = (Map<String,Map<String,Object>>) mlabel.get("OBJECTS");
    return new PDSLabel(version, metadata, objects);
  }

  public static void main(String[] args) {
    String label_name;
    if (args.length > 1) {
      label_name = args[1];
    } else {
      label_name = "samples/mola.lbl";
    }

    PDSLabel label = parseLabel(label_name);

    if (label == null) {
      System.out.println("couldn't parse " + label_name);
      return;
    }

    System.out.println("parsed a version " + label.getVersion() + " label from " + label_name);
    System.out.println();

    for (Map.Entry<String,PDSValue> datum : label.metadata.entrySet()) {
      String key = datum.getKey();

      PDSValue value = datum.getValue();
      System.out.println(key + ": " + value.toString());
    }
    System.out.println();

    for (Map.Entry<String,PDSObject> obj_entry : label.objects.entrySet()) {
      String name = obj_entry.getKey();
      System.out.println(name + " {");
      for (Map.Entry<String,PDSValue> attr : obj_entry.getValue().attributes.entrySet()) {
        System.out.println("  " + attr.getKey() + ": " + attr.getValue().toString());
      }
      System.out.println("}\n");
    }
  }
}
