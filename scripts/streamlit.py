
from sqlalchemy import create_engine, text
from sqlalchemy.orm import sessionmaker
from sqlalchemy.exc import OperationalError

import streamlit as st
import pandas as pd
import pandas
import plotly.express as px


@st.cache_resource
def conectar_mysql(username: str, password: str, host: str, dbname: str):
    try:
        # Define a URL de conexão para o MySQL
        connection_url = f"mysql+pymysql://{username}:{password}@{host}:3306/{dbname}"

        # Cria o motor (engine) de conexão
        engine = create_engine(connection_url)

        # Cria uma sessão
        Session = sessionmaker(bind=engine)
        session = Session()

        print("Conexão bem-sucedida!")

        # Retorna o engine e a sessão para futuras interações
        return engine, session

    except OperationalError as e:
        print(f"Erro ao conectar ao banco de dados: {e}")
        return None, None


@st.cache_data
def load_data(
    username= 'bruno',
    password= 'blurbang',
    host= 'fiap-f7.cqqlntsajwdo.us-east-1.rds.amazonaws.com',
    db= 'dados_sensores',
    header= ['id', 'temperature', 'humidity', 'timestamp', 'sensor_id'],
    query= 'select * from dados_sensores.dados_sensores'
):
    engine, _ = conectar_mysql(username, password, host, db)
    dados = None

    with engine.connect() as connection:
        dados = connection.execute(text(query)).fetchall()

    if dados:
        return pandas.DataFrame([dict(zip(header,d)) for d in dados])
    else:
        return pandas.DataFrame([])


# Configuração da página
st.set_page_config(page_title="Dashboard Sensores", layout="wide")

# Título do Dashboard
st.title("📊 Dashboard Interativo de Dados de Sensores")
st.markdown("Visualização dos dados de temperatura e umidade coletados.")

# Carregar os dados
data_load_state = st.text('Carregando dados...')
df_sensores = load_data()
data_load_state.text('Dados carregados com sucesso!')

if not df_sensores.empty:
    # Opção para exibir os dados brutos
    if st.checkbox('Mostrar dados brutos'):
        st.subheader('Dados Brutos')
        st.write(df_sensores)

    st.sidebar.header("Filtros")

    # Filtro por Sensor ID (mesmo que só haja um no exemplo, é bom ter)
    sensor_list = df_sensores['sensor_id'].unique()
    selected_sensor = st.sidebar.selectbox('Selecione o Sensor ID', sensor_list, index=0)

    # Filtrar DataFrame pelo sensor selecionado
    df_filtered_by_sensor = df_sensores[df_sensores['sensor_id'] == selected_sensor]

    # Filtro por intervalo de datas
    if not df_filtered_by_sensor.empty:
        min_date = df_filtered_by_sensor['timestamp'].min().date()
        max_date = df_filtered_by_sensor['timestamp'].max().date()

        date_range = st.sidebar.date_input(
            'Selecione o intervalo de datas',
            value=(min_date, max_date),
            min_value=min_date,
            max_value=max_date
        )

        # Garantir que date_range tenha dois elementos
        if len(date_range) == 2:
            start_date, end_date = date_range
            # Converter datas para datetime para comparação correta
            start_datetime = pd.to_datetime(start_date)
            end_datetime = pd.to_datetime(end_date) + pd.Timedelta(days=1) # Incluir o dia final

            # Filtrar DataFrame pelo intervalo de datas selecionado
            df_filtered = df_filtered_by_sensor[
                (df_filtered_by_sensor['timestamp'] >= start_datetime) & 
                (df_filtered_by_sensor['timestamp'] < end_datetime)
            ]
        else:
            st.sidebar.warning("Por favor, selecione um intervalo de datas válido.")
            df_filtered = df_filtered_by_sensor # Usar dados filtrados apenas por sensor se data for inválida
    else:
        df_filtered = df_filtered_by_sensor # Se não houver dados após filtro de sensor

    # --- Visualizações --- 
    st.header(f"Visualizações para o Sensor: {selected_sensor}")

    if not df_filtered.empty:
        # Gráfico de Linha: Temperatura ao longo do tempo
        st.subheader('Temperatura (°C) ao Longo do Tempo')
        fig_temp = px.line(df_filtered, x='timestamp', y='temperature', 
                           title='Variação da Temperatura',
                           labels={'timestamp': 'Data e Hora', 'temperature': 'Temperatura (°C)'})
        fig_temp.update_layout(xaxis_title='Data e Hora', yaxis_title='Temperatura (°C)')
        st.plotly_chart(fig_temp, use_container_width=True)

        # Gráfico de Linha: Umidade ao longo do tempo
        st.subheader('Umidade (%) ao Longo do Tempo')
        fig_hum = px.line(df_filtered, x='timestamp', y='humidity', 
                          title='Variação da Umidade',
                          labels={'timestamp': 'Data e Hora', 'humidity': 'Umidade (%)'},
                          color_discrete_sequence=px.colors.qualitative.Pastel)
        fig_hum.update_layout(xaxis_title='Data e Hora', yaxis_title='Umidade (%)')
        st.plotly_chart(fig_hum, use_container_width=True)

        # Exibir algumas estatísticas descritivas
        st.subheader("Estatísticas Descritivas")
        st.write(df_filtered[['temperature', 'humidity']].describe())

    else:
        st.warning("Nenhum dado encontrado para os filtros selecionados.")

else:
    st.error("Não foi possível carregar os dados. Verifique a conexão com o banco de dados ou o arquivo.")

# Rodapé
st.markdown("--- ")
st.markdown("Desenvolvido por Manus")
 