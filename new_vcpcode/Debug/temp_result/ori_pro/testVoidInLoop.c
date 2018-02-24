empty void  func1();

empty void  main();

empty int  Test_V;

empty void  func1()
{
  empty int  x = 1;

  empty int  result = 0;

  result = x+1;
}
empty void  main()
{
  empty int  x = 3;

  empty int  y = 4;

  empty int  z = 5;

  Test_V = x+y*z;
  while(Test_V<=24)
  {
    func1();
    Test_V = Test_V+1;
  }
}
