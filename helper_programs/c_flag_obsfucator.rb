require 'securerandom'

print "Enter flag: "
flag = gets.chomp
bytes = flag.bytes

# ----------------------------
# KEYS
# ----------------------------
key1 = SecureRandom.random_number(255) + 1
key2 = SecureRandom.random_number(255) + 1
rng_seed = SecureRandom.random_number(0xffffffff)
rng = Random.new(rng_seed)

# ----------------------------
# LEVEL 1: XOR
# ----------------------------
stage1 = bytes.map { |b| b ^ key1 }

# ----------------------------
# LEVEL 2: positional XOR
# ----------------------------
stage2 = stage1.each_with_index.map do |b, i|
  b ^ (key2 ^ ((i * 31) & 0xff))
end

# ----------------------------
# LEVEL 3: chunk permutation (FIXED MODEL)
# ----------------------------
chunk_size = 4
chunks = stage2.each_slice(chunk_size).to_a

# pad last chunk
last = chunks[-1]
if last.length < chunk_size
  last.concat(Array.new(chunk_size - last.length, 0))
end

perm = (0...chunks.length).to_a.shuffle(random: rng)

# encode: shuffled output
shuffled = perm.map { |i| chunks[i] }
flat = shuffled.flatten

# ----------------------------
# OUTPUT C CODE
# ----------------------------
puts "\n--- Generated C Code ---\n"

puts "#include <stdio.h>"

puts "void decrypt_flag() {"

puts "    unsigned char key1 = 0x#{key1.to_s(16).rjust(2,'0')};"
puts "    unsigned char key2 = 0x#{key2.to_s(16).rjust(2,'0')};"
puts "    unsigned int seed = 0x#{rng_seed.to_s(16)};"

puts "    int chunk_size = #{chunk_size};"
puts "    int chunk_count = #{chunks.length};"

puts "    int perm[] = { #{perm.join(", ")} };"

puts "    unsigned char data[] = {"
puts "        #{flat.map { |x| "0x#{x.to_s(16).rjust(2,'0')}" }.join(", ")}"
puts "    };"

puts "    int len = #{flat.length};"

puts <<~C

    unsigned char chunks[256][4];
    unsigned char stage1[512];
    unsigned char stage2[512];
    unsigned char out[512];

    int idx = 0;

    // ----------------------------
    // STEP 1: rebuild original chunks using perm
    // ----------------------------
    for(int i = 0; i < chunk_count; i++) {
        for(int j = 0; j < chunk_size; j++) {
            chunks[perm[i]][j] = data[idx++];
        }
    }

    // ----------------------------
    // STEP 2: flatten in correct order
    // ----------------------------
    idx = 0;
    for(int i = 0; i < chunk_count; i++) {
        for(int j = 0; j < chunk_size; j++) {
            stage1[idx++] = chunks[i][j];
        }
    }

    // ----------------------------
    // STEP 3: undo positional XOR
    // ----------------------------
    for(int i = 0; i < len; i++) {
        stage2[i] = stage1[i] ^ (key2 ^ ((i * 31) & 0xff));
    }

    // ----------------------------
    // STEP 4: undo base XOR
    // ----------------------------
    for(int i = 0; i < len; i++) {
        out[i] = stage2[i] ^ key1;
    }

    // ----------------------------
    // SAFE OUTPUT (NO %s BUG)
    // ----------------------------
    for(int i = 0; i < len - 2; i++) {
        printf("%c", out[i]);
    }
    printf("\\n");

}
C

puts "\nCall decrypt_flag(); in main()."