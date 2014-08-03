package org.aperiodic;

import java.util.Date;

enum ValueType {
    INT
  , FLOAT
  , STRING
  , DATE
  , TUPLE
}

public class PDSValue {
  private ValueType type;
  private int intVal;
  private float floatVal;
  private String stringVal;
  private Date dateVal;
  private PDSValue[] tupleVal;

  public PDSValue(int intVal) {
    this.type = ValueType.INT;
    this.intVal = intVal;
  }

  public PDSValue(float floatVal) {
    this.type = ValueType.FLOAT;
    this.floatVal = floatVal;
  }

  public PDSValue(String stringVal) {
    this.type = ValueType.STRING;
    this.stringVal = stringVal;
  }

  public PDSValue(Date dateVal) {
    this.type = ValueType.DATE;
    this.dateVal = dateVal;
  }

  public PDSValue(PDSValue[] tupleVal) {
    this.type = ValueType.TUPLE;
    this.tupleVal = tupleVal;
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
}
