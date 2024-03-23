import random
import string

def generate_cnpj():
    cnpj_digits = ''.join(random.choices(string.digits, k=14))
    return f'{cnpj_digits[:2]}.{cnpj_digits[2:5]}.{cnpj_digits[5:8]}/{cnpj_digits[8:12]}-{cnpj_digits[12:]}'

def generate_input_file(entries, filename):
    with open(filename, 'w') as file:
        containers = []
        cnpjs = []
        
        # Generate 1000 entries
        file.write(f"{entries}\n")
        for _ in range(entries):
            container = ''.join(random.choices(string.ascii_uppercase + string.digits, k=10))
            cnpj = generate_cnpj()
            weight = random.randint(10000, 100000)
            containers.append(container)
            cnpjs.append(cnpj)
            file.write(f"{container} {cnpj} {weight}\n")
        
        # Generate 10000 inspection entries using containers and some repeated cnpjs from the first 10000 entries
        file.write(f"10000\n")
        for _ in range(10000):
            container = random.choice(containers)
           # cnpj = random.choice(cnpjs) if random.random() < 0.2 else generate_cnpj()
            cnpj = cnpjs[containers.index(container)] if random.random() < 0.2 else generate_cnpj()
            weight = random.randint(10000, 100000)
            file.write(f"{container} {cnpj} {weight}\n")

# Change '/path/to/input_1000.txt' to the desired full path
generate_input_file(50000, 'input_50000.txt')
