import json
import argparse
import matplotlib.pyplot as plt
import pandas as pd


def main():
    parser = argparse.ArgumentParser(
        description="Visualisation ECG avec détection des pics P, Q, R, S, T (format CSV: colonnes=leads)"
    )

    parser.add_argument(
        "--csv",
        required=True,
        help="Chemin vers le fichier CSV ECG"
    )
    parser.add_argument(
        "--json",
        required=True,
        help="Chemin vers le fichier JSON des pics"
    )
    parser.add_argument(
        "--lead",
        default="1",
        help="Index du lead à afficher (0..11). Par défaut: 1 (lead II)"
    )
    parser.add_argument(
        "--samples",
        type=int,
        default=5000,
        help="Nombre d'échantillons à afficher (par défaut: 5000)"
    )

    args = parser.parse_args()

    # Charger le signal ECG
    # CSV attendu: lignes = échantillons, colonnes = leads (0..11), sans header
    df = pd.read_csv(args.csv, delimiter=",", header=None)

    lead_idx = int(args.lead)
    if lead_idx < 0 or lead_idx >= df.shape[1]:
        raise ValueError(f"Lead index {lead_idx} invalide (0..{df.shape[1]-1})")

    ecg_signal = df.iloc[:args.samples, lead_idx].values

    # Charger le fichier JSON contenant les pics
    with open(args.json, "r") as f:
        data = json.load(f)

    peaks = data.get("peaks", {})

    def filter_peaks(name):
        return [p for p in peaks.get(name, []) if p < args.samples]

    p_peaks = filter_peaks("P")
    q_peaks = filter_peaks("Q")
    r_peaks = filter_peaks("R")
    s_peaks = filter_peaks("S")
    t_peaks = filter_peaks("T")

    # Tracer le signal ECG
    plt.figure(figsize=(14, 6))
    plt.plot(ecg_signal, label=f"ECG (lead index {lead_idx})", color="blue")

    # Superposer les pics (afficher seulement si non vide)
    if p_peaks:
        plt.scatter(p_peaks, ecg_signal[p_peaks], color="orange", label="P", zorder=3)
    if q_peaks:
        plt.scatter(q_peaks, ecg_signal[q_peaks], color="purple", label="Q", zorder=3)
    if r_peaks:
        plt.scatter(r_peaks, ecg_signal[r_peaks], color="red", label="R", zorder=3)
    if s_peaks:
        plt.scatter(s_peaks, ecg_signal[s_peaks], color="green", label="S", zorder=3)
    if t_peaks:
        plt.scatter(t_peaks, ecg_signal[t_peaks], color="magenta", label="T", zorder=3)

    # Mise en forme
    plt.xlabel("Temps (échantillons)")
    plt.ylabel("Amplitude ECG")
    plt.title("ECG avec détection des pics (P, Q, R, S, T)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()