fun somma(num1, num2) => { <- num1 + num2; }
fun quadrato(num) => { <- num * num; }

fun main() => {
  @var1 = 3 * 2 + 4;
  @var2 = var1 - 3;

  print_int (somma (var1, var2));
  print_int (quadrato (var1));
  <- 0;
}