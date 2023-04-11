static union { int x; char endian; } little = { 1 };
int main() { return !little.endian; }
