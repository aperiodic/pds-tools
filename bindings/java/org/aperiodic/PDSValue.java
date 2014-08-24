package org.aperiodic;

import java.util.Date;

enum ValueType {
    INT
  , FLOAT
  , STRING
  , DATE
  , TUPLE
}

public class PDSValue extends Object {
  private ValueType type;
  private int intVal;
  private float floatVal;
  private String stringVal;
  private Date dateVal;
  private PDSValue[] tupleVal;

  public PDSValue(Object legalValue) {
    Class val_class = legalValue.getClass();
    if (val_class == java.lang.Integer.class) {
      this.type = ValueType.INT;
      this.intVal = ((Integer)legalValue).intValue();
    } else if (val_class == java.lang.Float.class) {
      this.type = ValueType.FLOAT;
      this.floatVal = ((Float)legalValue).floatValue();
    } else if (val_class == java.lang.String.class) {
      this.type = ValueType.STRING;
      this.stringVal = (String) legalValue;
    } else if (val_class == Date.class) {
      this.type = ValueType.DATE;
      this.dateVal = (Date) legalValue;
    } else if (val_class.isArray()) {
      Class comp_class = val_class.getComponentType();
      if (comp_class != this.getClass()) {
          throw new IllegalArgumentException("tuple value's component type is not a PDSValue but "
                                             + comp_class);
      }
      this.type = ValueType.TUPLE;
      this.tupleVal = (PDSValue[]) legalValue;
    } else {
      throw new IllegalArgumentException("argument's class " + val_class
                                         + " is not a legal PDSValue");
    }
  }

  Integer getIntVal() {
    if (this.type != ValueType.INT) {
      return null;
    } else {
      return intVal;
    }
  }

  Float getFloatVal() {
    if (this.type != ValueType.FLOAT) {
      return null;
    } else {
      return floatVal;
    }
  }

  String getStringVal() {
    if (this.type != ValueType.STRING) {
      return null;
    } else {
      return stringVal;
    }
  }

  Date getDateVal() {
    if (this.type != ValueType.DATE) {
      return null;
    } else {
      return dateVal;
    }
  }

  PDSValue[] getTupleVal() {
    if (this.type != ValueType.TUPLE) {
      return null;
    } else {
      return tupleVal;
    }
  }

  Object getValue() {
    switch (this.type) {
      case INT:
        return intVal;
      case FLOAT:
        return floatVal;
      case STRING:
        return stringVal;
      case DATE:
        return dateVal;
      case TUPLE:
        return tupleVal;
      default:
        return null;
    }
  }

  public String toString() {
    return this.getValue().toString();
  }
}
