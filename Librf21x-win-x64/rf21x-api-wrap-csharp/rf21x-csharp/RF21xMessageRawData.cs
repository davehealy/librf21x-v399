//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.5
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class RF21xMessageRawData : global::System.IDisposable, global::System.Collections.IEnumerable
    , global::System.Collections.Generic.IList<byte>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal RF21xMessageRawData(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(RF21xMessageRawData obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~RF21xMessageRawData() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          rf21xPINVOKE.delete_RF21xMessageRawData(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public RF21xMessageRawData(global::System.Collections.ICollection c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (byte element in c) {
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

  public byte this[int index]  {
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

  public void CopyTo(byte[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(byte[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, byte[] array, int arrayIndex, int count)
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

  global::System.Collections.Generic.IEnumerator<byte> global::System.Collections.Generic.IEnumerable<byte>.GetEnumerator() {
    return new RF21xMessageRawDataEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new RF21xMessageRawDataEnumerator(this);
  }

  public RF21xMessageRawDataEnumerator GetEnumerator() {
    return new RF21xMessageRawDataEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class RF21xMessageRawDataEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<byte>
  {
    private RF21xMessageRawData collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public RF21xMessageRawDataEnumerator(RF21xMessageRawData collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public byte Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (byte)currentObject;
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
    rf21xPINVOKE.RF21xMessageRawData_Clear(swigCPtr);
  }

  public void Add(byte x) {
    rf21xPINVOKE.RF21xMessageRawData_Add(swigCPtr, x);
  }

  private uint size() {
    uint ret = rf21xPINVOKE.RF21xMessageRawData_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = rf21xPINVOKE.RF21xMessageRawData_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    rf21xPINVOKE.RF21xMessageRawData_reserve(swigCPtr, n);
  }

  public RF21xMessageRawData() : this(rf21xPINVOKE.new_RF21xMessageRawData__SWIG_0(), true) {
  }

  public RF21xMessageRawData(RF21xMessageRawData other) : this(rf21xPINVOKE.new_RF21xMessageRawData__SWIG_1(RF21xMessageRawData.getCPtr(other)), true) {
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public RF21xMessageRawData(int capacity) : this(rf21xPINVOKE.new_RF21xMessageRawData__SWIG_2(capacity), true) {
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  private byte getitemcopy(int index) {
    byte ret = rf21xPINVOKE.RF21xMessageRawData_getitemcopy(swigCPtr, index);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private byte getitem(int index) {
    byte ret = rf21xPINVOKE.RF21xMessageRawData_getitem(swigCPtr, index);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, byte val) {
    rf21xPINVOKE.RF21xMessageRawData_setitem(swigCPtr, index, val);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(RF21xMessageRawData values) {
    rf21xPINVOKE.RF21xMessageRawData_AddRange(swigCPtr, RF21xMessageRawData.getCPtr(values));
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public RF21xMessageRawData GetRange(int index, int count) {
    global::System.IntPtr cPtr = rf21xPINVOKE.RF21xMessageRawData_GetRange(swigCPtr, index, count);
    RF21xMessageRawData ret = (cPtr == global::System.IntPtr.Zero) ? null : new RF21xMessageRawData(cPtr, true);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, byte x) {
    rf21xPINVOKE.RF21xMessageRawData_Insert(swigCPtr, index, x);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, RF21xMessageRawData values) {
    rf21xPINVOKE.RF21xMessageRawData_InsertRange(swigCPtr, index, RF21xMessageRawData.getCPtr(values));
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    rf21xPINVOKE.RF21xMessageRawData_RemoveAt(swigCPtr, index);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    rf21xPINVOKE.RF21xMessageRawData_RemoveRange(swigCPtr, index, count);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public static RF21xMessageRawData Repeat(byte value, int count) {
    global::System.IntPtr cPtr = rf21xPINVOKE.RF21xMessageRawData_Repeat(value, count);
    RF21xMessageRawData ret = (cPtr == global::System.IntPtr.Zero) ? null : new RF21xMessageRawData(cPtr, true);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    rf21xPINVOKE.RF21xMessageRawData_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    rf21xPINVOKE.RF21xMessageRawData_Reverse__SWIG_1(swigCPtr, index, count);
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, RF21xMessageRawData values) {
    rf21xPINVOKE.RF21xMessageRawData_SetRange(swigCPtr, index, RF21xMessageRawData.getCPtr(values));
    if (rf21xPINVOKE.SWIGPendingException.Pending) throw rf21xPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Contains(byte value) {
    bool ret = rf21xPINVOKE.RF21xMessageRawData_Contains(swigCPtr, value);
    return ret;
  }

  public int IndexOf(byte value) {
    int ret = rf21xPINVOKE.RF21xMessageRawData_IndexOf(swigCPtr, value);
    return ret;
  }

  public int LastIndexOf(byte value) {
    int ret = rf21xPINVOKE.RF21xMessageRawData_LastIndexOf(swigCPtr, value);
    return ret;
  }

  public bool Remove(byte value) {
    bool ret = rf21xPINVOKE.RF21xMessageRawData_Remove(swigCPtr, value);
    return ret;
  }

}