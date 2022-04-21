static union { int x; char endian; } little = { 1 };
main() { exit(!little.endian); }
