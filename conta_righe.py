import os

def count_lines_in_file(file_path):
    """Conta le righe di un singolo file."""
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            return sum(1 for _ in f)
    except Exception as e:
        print(f"Errore leggendo {file_path}: {e}")
        return 0

def count_lines_in_directory(directory):
    """Conta le righe di tutti i file .c e .h nella directory e nelle sottodirectory."""
    total_lines = 0
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(('.c', '.h')):
                file_path = os.path.join(root, file)
                lines = count_lines_in_file(file_path)
                print(f"{file_path}: {lines} righe")
                total_lines += lines
    return total_lines

if __name__ == "__main__":
    directory = os.getcwd()  # Usa la cartella corrente
    total = count_lines_in_directory(directory)
    print(f"\nTotale righe di codice: {total}")
