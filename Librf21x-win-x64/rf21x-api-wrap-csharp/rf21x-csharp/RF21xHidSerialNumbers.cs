//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.5
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class RF21xHidSerialNumbers : global::System.IDisposable, global::System.Collections.IEnumerable
    , global::System.Collections.Generic.IList<string>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal RF21xHidSerialNumbers(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(RF21xHidSerialNumbers obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~RF21xHidSerialNumbers() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          rf21xPINVOKE.delete_RF21xHidSerialNumbers(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public RF21xHidSerialNumbers(global::System.Collections.ICollection c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (string element in c) {
      this.Add(element);
    }
  }

  public bool IsFixedSize {
    get {
      return false;
    }
  }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public string this[int index]  {
    get {
      return getitem(index);
    }
    set {
      setitem(index, value);
    }
  }

  public int Capacity {
    get {
      return (int)capacity();
    }
    set {
      if (value < size())
        throw new global::System.ArgumentOutOfRangeException("Capacity");
      reserve((uint)value);
    }
  }

  public int Count {
    get {
      return (int)size();
    }
  }

  public bool IsSynchronized {
    get {
      return false;
    }
  }

  public void CopyTo(string[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(string[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, string[] array, int arrayIndex, int count)
  {
    if (array == null)
      throw new global::System.ArgumentNullException("array");
    if (index < 0)
      throw new global::System.ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new global::System.ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new global::System.ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new global::System.ArgumentException("Multi dimensional array.", "array");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new global::System.ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

  global::System.Collections.Generic.IEnumerator<string> global::System.Collections.Generic.IEnumerable<string>.GetEnumerator() {
    return new RF21xHidSerialNumbersEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new RF21xHidSerialNumbersEnumerator(this);
  }

  public RF21xHidSerialNumbersEnumerator GetEnumerator() {
    return new RF21xHidSerialNumbersEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class RF21xHidSerialNumbersEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<string>
  {
    private RF21xHidSerialNumbers collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public RF21xHidSerialNumbersEnumerator(RF21xHidSerialNumbers collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public string Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (string)currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object global::System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    public bool MoveNext() {
      int size = collectionRef.Count;
      bool moveOkay = (currentIndex+1 < size) && (size == currentSize);
      if (moveOkay) {
        currentIndex++;
        currentObject = collectionRef[currentIndex];
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new global::System.InvalidOperationException("Collection modified.");
      }
    }

    public void Dispose() {
        currentIndex = -1;
        currentObject = null;
    }
  }

  public void Clear() {
    rf21xPINVOKE.RF21xHidSerialNumbers_Clear(swigCPtr);
  }

  public void Add(string x) {
    rf21xPINVOKE.RF21xHidSerialNumbers_Add(swigCPtr, x);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  private uint size() {
    uint ret = rf21xPINVOKE.RF21xHidSerialNumbers_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = rf21xPINVOKE.RF21xHidSerialNumbers_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    rf21xPINVOKE.RF21xHidSerialNumbers_reserve(swigCPtr, n);
  }

  public RF21xHidSerialNumbers() : this(rf21xPINVOKE.new_RF21xHidSerialNumbers__SWIG_0(), true) {
  }

  public RF21xHidSerialNumbers(RF21xHidSerialNumbers other) : this(rf21xPINVOKE.new_RF21xHidSerialNumbers__SWIG_1(RF21xHidSerialNumbers.getCPtr(other)), true) {
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public RF21xHidSerialNumbers(int capacity) : this(rf21xPINVOKE.new_RF21xHidSerialNumbers__SWIG_2(capacity), true) {
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  private string getitemcopy(int index) {
    string ret = rf21xPINVOKE.RF21xHidSerialNumbers_getitemcopy(swigCPtr, index);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private string getitem(int index) {
    string ret = rf21xPINVOKE.RF21xHidSerialNumbers_getitem(swigCPtr, index);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, string val) {
    rf21xPINVOKE.RF21xHidSerialNumbers_setitem(swigCPtr, index, val);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(RF21xHidSerialNumbers values) {
    rf21xPINVOKE.RF21xHidSerialNumbers_AddRange(swigCPtr, RF21xHidSerialNumbers.getCPtr(values));
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public RF21xHidSerialNumbers GetRange(int index, int count) {
    global::System.IntPtr cPtr = rf21xPINVOKE.RF21xHidSerialNumbers_GetRange(swigCPtr, index, count);
    RF21xHidSerialNumbers ret = (cPtr == global::System.IntPtr.Zero) ? null : new RF21xHidSerialNumbers(cPtr, true);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, string x) {
    rf21xPINVOKE.RF21xHidSerialNumbers_Insert(swigCPtr, index, x);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, RF21xHidSerialNumbers values) {
    rf21xPINVOKE.RF21xHidSerialNumbers_InsertRange(swigCPtr, index, RF21xHidSerialNumbers.getCPtr(values));
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    rf21xPINVOKE.RF21xHidSerialNumbers_RemoveAt(swigCPtr, index);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    rf21xPINVOKE.RF21xHidSerialNumbers_RemoveRange(swigCPtr, index, count);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public static RF21xHidSerialNumbers Repeat(string value, int count) {
    global::System.IntPtr cPtr = rf21xPINVOKE.RF21xHidSerialNumbers_Repeat(value, count);
    RF21xHidSerialNumbers ret = (cPtr == global::System.IntPtr.Zero) ? null : new RF21xHidSerialNumbers(cPtr, true);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    rf21xPINVOKE.RF21xHidSerialNumbers_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    rf21xPINVOKE.RF21xHidSerialNumbers_Reverse__SWIG_1(swigCPtr, index, count);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, RF21xHidSerialNumbers values) {
    rf21xPINVOKE.RF21xHidSerialNumbers_SetRange(swigCPtr, index, RF21xHidSerialNumbers.getCPtr(values));
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Contains(string value) {
    bool ret = rf21xPINVOKE.RF21xHidSerialNumbers_Contains(swigCPtr, value);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int IndexOf(string value) {
    int ret = rf21xPINVOKE.RF21xHidSerialNumbers_IndexOf(swigCPtr, value);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int LastIndexOf(string value) {
    int ret = rf21xPINVOKE.RF21xHidSerialNumbers_LastIndexOf(swigCPtr, value);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public bool Remove(string value) {
    bool ret = rf21xPINVOKE.RF21xHidSerialNumbers_Remove(swigCPtr, value);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}