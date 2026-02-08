import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

csv_path = os.path.join(BASE_DIR, "hll_results.csv")
images_dir = os.path.join(BASE_DIR, "images")

os.makedirs(images_dir, exist_ok=True)

try:
    df = pd.read_csv(csv_path)
except FileNotFoundError:
    print("Ошибка: Файл hll_results.csv не найден. Сначала запустите stage2.")
    exit()

stats = df.groupby('step').agg({
    'true_count': 'mean',
    'est_count': ['mean', 'std']
}).reset_index()

stats.columns = ['step', 'true_mean', 'est_mean', 'est_std']

# ==============================
# График 1: Сравнение
# ==============================
plt.figure(figsize=(10, 6))
plt.plot(stats['step'], stats['true_mean'], 'g-', label='True Cardinality', linewidth=2)
plt.plot(stats['step'], stats['est_mean'], 'b--', label='HLL Estimate')
plt.xlabel('Stream Size')
plt.ylabel('Cardinality')
plt.title('True Cardinality vs HLL Estimate')
plt.legend()
plt.grid(True)

graph1_path = os.path.join(images_dir, "graph1_comparison.png")
plt.savefig(graph1_path)
plt.close()
print(f"Сохранен {graph1_path}")

# ==============================
# График 2: Область неопределенности
# ==============================
plt.figure(figsize=(10, 6))
plt.plot(stats['step'], stats['est_mean'], 'b-', label='Estimate Mean')
plt.fill_between(
    stats['step'],
    stats['est_mean'] - stats['est_std'],
    stats['est_mean'] + stats['est_std'],
    alpha=0.2,
    label='Std Dev Range'
)
plt.plot(stats['step'], stats['true_mean'], 'g--', label='True Value', alpha=0.6)
plt.xlabel('Stream Size')
plt.ylabel('Cardinality')
plt.title('HLL Estimation Uncertainty')
plt.legend()
plt.grid(True)

graph2_path = os.path.join(images_dir, "graph2_uncertainty.png")
plt.savefig(graph2_path)
plt.close()
print(f"Сохранен {graph2_path}")