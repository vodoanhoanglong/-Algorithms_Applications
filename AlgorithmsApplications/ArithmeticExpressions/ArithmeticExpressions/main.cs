using System;
using System.Collections.Generic;

namespace ArithmeticExpressions
{
    class main
    {
        static void Main(string[] args)
        {
          
            Console.WriteLine("---Nhap vao bieu thuc can tinh nhanh vi du: (-67) * (1 - 301) - 301 * 67");
            Console.WriteLine("---Nhap vao bieu thuc nay de minh hoa ro hon bieu thuc tinh nhanh cac so nguyen: ");
            Console.WriteLine("(a * (10 + 5)) / ((2 + 2) * 3) + b * (3 + 5) + 2 * 2");
            Console.WriteLine("---Luu y cach 1 khoang trang giua cac bieu thuc!!!");
            Console.Write("- Nhap bieu thuc cua ban: ");
            String expression = Console.ReadLine();
            var tokens = Parser.Parse(expression);
            var asTreeCreator = new ASTreeCreator(tokens);
            asTreeCreator.BuildASTree();
            var asTreeCalculator = new ASTreeCalculator(asTreeCreator.Root);
            var result = asTreeCalculator.GetSimplifiedExpression();
            Console.Write("- Ket qua la: ");
            foreach (var elem in result)
            {
                Console.Write("{0} ", elem);
            }
        }
    }
}
