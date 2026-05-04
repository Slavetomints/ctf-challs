use std::io;

struct VM {
    tape: [u8; 64],
    ptr: usize,
    pc: usize,
    prog: Vec<u8>,
    input: Vec<u8>,
    input_ptr: usize,
    checksum: u32,
}

impl VM {
    fn new(prog: Vec<u8>, input: Vec<u8>) -> Self {
        Self {
            tape: [0; 64],
            ptr: 0,
            pc: 0,
            prog,
            input,
            input_ptr: 0,
            checksum: 0x12345678,
        }
    }

    fn step(&mut self) -> bool {
        if self.pc >= self.prog.len() {
            return false;
        }

        match self.prog[self.pc] {
            b'>' => self.ptr = (self.ptr + 1) % 64,
            b'<' => self.ptr = (self.ptr + 63) % 64,
            b'+' => self.tape[self.ptr] = self.tape[self.ptr].wrapping_add(1),
            b'-' => self.tape[self.ptr] = self.tape[self.ptr].wrapping_sub(1),

            b',' => {
                if self.input_ptr < self.input.len() {
                    self.tape[self.ptr] = self.input[self.input_ptr];
                    self.input_ptr += 1;
                }
            }

            b'#' => {
                self.checksum = self
                    .checksum
                    .wrapping_mul(31)
                    .wrapping_add(self.tape[self.ptr] as u32);
            }

            _ => {}
        }

        self.pc += 1;
        true
    }

    fn run(&mut self) {
        self.ptr = 0;
        self.pc = 0;
        while self.step() {}
    }
    //++++++++++++++++++++++++++++++++++++++++++>+++++++++++++++++++>+++++++++++++++++++++++++++++++++++++++++++++++>#>#>#
    fn valid(&self) -> bool {
        self.tape[0] == 42
            && self.tape[1] == 19
            && self.tape[2] == 47
            && self.checksum == 0x789a7688
    }
}

fn main() {
    println!("== Minty VM ==");

    // Read program
    println!("program> ");
    let mut prog = String::new();
    io::stdin().read_line(&mut prog).unwrap();
    let prog = prog
        .trim()
        .chars()
        .filter(|c| !c.is_whitespace())
        .map(|c| c as u8)
        .collect::<Vec<u8>>();
    // Optional input (for ',' instruction)
    println!("input> ");
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    let input = input.trim().as_bytes().to_vec();

    if prog.len() > 256 {
        println!("program too long");
        return;
    }

    let allowed = b"><+-.,[]#";
    if !prog.iter().all(|c| allowed.contains(c)) {
        println!("invalid character detected");
        return;
    }

    let mut vm = VM::new(prog, input);
    vm.run();

    println!("--- VM HALT ---");
    println!("tape[0] = {}", vm.tape[0]);
    println!("tape[1] = {}", vm.tape[1]);
    println!("tape[2] = {}", vm.tape[2]);
    println!("checksum = {:08x}", vm.checksum);

    if vm.valid() {
        decrypt_flag();
    } else {
        println!("invalid state");
    }
}

fn decrypt_flag() {
    let key1: u8 = 0x37;
    let key2: u8 = 0xa7;
    let _seed: u32 = 0x1193da4f;

    let chunk_size = 4;
    let chunk_count = 11;

    let perm: Vec<usize> = vec![5, 6, 4, 0, 7, 1, 3, 2, 9, 10, 8];

    let data: Vec<u8> = vec![
        0x8e, 0x78, 0x54, 0x68, 0x2b, 0xfd, 0xd0, 0xe7, 0x53, 0xef, 0x86, 0x89, 0xc3, 0xdb, 0xe1,
        0x98, 0x91, 0x21, 0x5d, 0x0, 0xb8, 0x48, 0x7e, 0xf, 0xdd, 0x4d, 0x50, 0xe, 0x13, 0xb3,
        0xe4, 0x84, 0x84, 0x82, 0x66, 0x7f, 0x10, 0x1a, 0x0, 0x0, 0x18, 0x38, 0xc5, 0xe4,
    ];

    let len = 44;

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

    let mut end = out.len();

    if let Some(pos) = out.iter().position(|&b| b == b'}') {
        end = pos + 1;
    }

    println!("{}", String::from_utf8_lossy(&out[..end]));
}
