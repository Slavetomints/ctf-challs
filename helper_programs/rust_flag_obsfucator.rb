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
# LEVEL 3: chunk permutation
# ----------------------------
chunk_size = 4
chunks = stage2.each_slice(chunk_size).to_a

last = chunks[-1]
if last.length < chunk_size
  last.concat(Array.new(chunk_size - last.length, 0))
end

perm = (0...chunks.length).to_a.shuffle(random: rng)

shuffled = perm.map { |i| chunks[i] }
flat = shuffled.flatten

# ----------------------------
# OUTPUT RUST CODE
# ----------------------------
puts "\n--- Generated Rust Code ---\n"

puts "fn decrypt_flag() {"
puts "    let key1: u8 = 0x#{key1.to_s(16)};"
puts "    let key2: u8 = 0x#{key2.to_s(16)};"
puts "    let seed: u32 = 0x#{rng_seed.to_s(16)};"
puts ""
puts "    let chunk_size = #{chunk_size};"
puts "    let chunk_count = #{chunks.length};"
puts ""
puts "    let perm: Vec<usize> = vec![#{perm.join(", ")}];"
puts ""
puts "    let data: Vec<u8> = vec!["
puts "        #{flat.map { |x| "0x#{x.to_s(16)}" }.join(", ")}"
puts "    ];"
puts ""
puts "    let len = #{flat.length};"
puts ""
puts <<~RUST
    let mut chunks: Vec<Vec<u8>> = vec![vec![0; chunk_size]; chunk_count];
    let mut stage1 = vec![0u8; len];
    let mut stage2 = vec![0u8; len];
    let mut out = vec![0u8; len];

    let mut idx = 0;

    // ----------------------------
    // STEP 1: unpermute chunks
    // ----------------------------
    for i in 0..chunk_count {
        for j in 0..chunk_size {
            chunks[perm[i]][j] = data[idx];
            idx += 1;
        }
    }

    // ----------------------------
    // STEP 2: flatten
    // ----------------------------
    idx = 0;
    for i in 0..chunk_count {
        for j in 0..chunk_size {
            stage1[idx] = chunks[i][j];
            idx += 1;
        }
    }

    // ----------------------------
    // STEP 3: undo positional XOR
    // ----------------------------
    for i in 0..len {
        stage2[i] = stage1[i] ^ (key2 ^ ((i * 31) as u8));
    }

    // ----------------------------
    // STEP 4: undo base XOR
    // ----------------------------
    for i in 0..len {
        out[i] = stage2[i] ^ key1;
    }

    // ----------------------------
    // output
    // ----------------------------
    let result = String::from_utf8_lossy(&out);
    println!("{}", result);;
}
RUST

puts "\nCall decrypt_flag();"