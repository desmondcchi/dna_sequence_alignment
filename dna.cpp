#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

const double kMatchScore = 1.0;
const double kInsertScore = -0.2;
const double kDeleteScore = -0.2;

double score_table[4][4] = {
    {1, -0.1, -0.1, -0.15},
    {-0.1, 1, -0.15, -0.1},
    {-0.1, -0.15, 1, -0.1},
    {-0.15, -0.1, -0.1, 1},
};

int main() {
  std::unordered_map<char, int> dna_char_to_int;
  dna_char_to_int['A'] = 0;
  dna_char_to_int['G'] = 1;
  dna_char_to_int['C'] = 2;
  dna_char_to_int['T'] = 3;

  std::string x_str_raw = "";  // GTGAACGCTGGCGGCGTGCT
  std::string y_str_raw = "";  // AGCTAATACCCCATACGTTC
  std::cin >> x_str_raw >> y_str_raw;
  std::string x_str = ' ' + x_str_raw;
  std::string y_str = ' ' + y_str_raw;

  int row_len = y_str.length();
  int col_len = x_str.length();
  double dp[row_len][col_len];
  dp[0][0] = 0;
  for (int c = 1; c < col_len; c++) {
    dp[0][c] = dp[0][c - 1] + kInsertScore;
  }

  for (int r = 1; r < row_len; r++) {
    dp[r][0] = dp[r - 1][0] + kDeleteScore;

    for (int c = 1; c < col_len; c++) {
      double deletion = dp[r - 1][c] + kDeleteScore;
      double insertion = dp[r][c - 1] + kInsertScore;
      double match =
          dp[r - 1][c - 1] +
          score_table[dna_char_to_int[y_str[r]]][dna_char_to_int[x_str[c]]];
      dp[r][c] = std::max(std::max(deletion, insertion), match);
    }
  }

  // Print out table
  // for (int r = 0; r < row_len; r++) {
  //   for (int c = 0; c < col_len; c++) {
  //     std::cout << dp[r][c] << " ";
  //   }
  //   std::cout << "\n";
  // }

  // Maximum similarity score
  std::cout << dp[row_len - 1][col_len - 1] << "\n";

  // Back tracking
  // Precendence (starting at dp[0][0]): Diagonal (match/mismatch) > Horizontal
  // (deletion) > Vertical (insertion)
  // Precedence (starting at dp[row_len -1][col_len - 1]): Vertical (insertion)
  // > Horizontal (deletion) > Diagonal (match/mismatch)
  int r = row_len - 1;
  int c = col_len - 1;
  std::string align_x = "";
  std::string align_y = "";
  while (r > 0 || c > 0) {
    // Insertion (Vertical)
    if (dp[r][c] == dp[r - 1][c] + kInsertScore) {
      align_x = "-" + align_x;
      align_y = y_str[r] + align_y;
      r--;
    }
    // Deletion (Horizontal)
    else if (dp[r][c] == dp[r][c - 1] + kDeleteScore) {
      align_x = x_str[c] + align_x;
      align_y = "-" + align_y;
      c--;
    }
    // Match/mismatch
    else if (dp[r][c] == dp[r - 1][c - 1] +
                             score_table[dna_char_to_int[y_str[r]]]
                                        [dna_char_to_int[x_str[c]]]) {
      align_x = x_str[c] + align_x;
      align_y = y_str[r] + align_y;
      r--;
      c--;
    }
  }

  std::cout << align_x << "\n" << align_y << "\n";
}
